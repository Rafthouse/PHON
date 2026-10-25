#include "SpectrumAnalyzer.h"

// =============================================================================
SpectrumAnalyzer::SpectrumAnalyzer()
{
    magnitudesDb_.fill (floorDb);
}

void SpectrumAnalyzer::prepare (double sampleRate)
{
    sampleRate_ = sampleRate;
    ring_.assign ((size_t) fifo_.getTotalSize(), 0.0f);
    fifo_.reset();
    magnitudesDb_.fill (floorDb);
}

// =============================================================================
static float monoSample (const juce::AudioBuffer<float>& buffer, int sample) noexcept
{
    float sum = 0.0f;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        sum += buffer.getReadPointer (ch)[sample];

    return sum / (float) buffer.getNumChannels();
}

void SpectrumAnalyzer::push (const juce::AudioBuffer<float>& buffer) noexcept
{
    if (buffer.getNumChannels() == 0 || ring_.empty())
        return;

    int start1 = 0, size1 = 0, start2 = 0, size2 = 0;
    fifo_.prepareToWrite (buffer.getNumSamples(), start1, size1, start2, size2);

    int sample = 0;

    for (int i = 0; i < size1; ++i, ++sample)
        ring_[(size_t) (start1 + i)] = monoSample (buffer, sample);

    for (int i = 0; i < size2; ++i, ++sample)
        ring_[(size_t) (start2 + i)] = monoSample (buffer, sample);

    fifo_.finishedWrite (size1 + size2);
}

// =============================================================================
void SpectrumAnalyzer::pullSpectrum()
{
    const auto ready = fifo_.getNumReady();

    if (ready < fftSize)
        return;

    int start1 = 0, size1 = 0, start2 = 0, size2 = 0;
    fifo_.prepareToRead (ready, start1, size1, start2, size2);

    // Keep only the newest fftSize samples; older ones are discarded.
    const auto offset = ready - fftSize;

    for (int k = 0; k < fftSize; ++k)
    {
        const auto idx = offset + k;
        const auto pos = idx < size1 ? start1 + idx : start2 + (idx - size1);
        fftData_[(size_t) k] = ring_[(size_t) pos];
    }

    fifo_.finishedRead (ready);

    window_.multiplyWithWindowingTable (fftData_.data(), (size_t) fftSize);
    std::fill (fftData_.begin() + fftSize, fftData_.end(), 0.0f);
    fft_.performFrequencyOnlyForwardTransform (fftData_.data());

    constexpr auto norm = 2.0f / (float) fftSize;

    for (int b = 0; b < numBins; ++b)
    {
        const auto target = juce::Decibels::gainToDecibels (fftData_[(size_t) b] * norm, floorDb);
        auto& current = magnitudesDb_[(size_t) b];

        // Fast attack, slow release.
        current = target > current ? target : current + (target - current) * 0.3f;
    }
}
