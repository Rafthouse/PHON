#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Spectrum tap for the PRE / POST display.
 *
 *  push() runs on the audio thread: it mixes the block to mono and writes it
 *  into a lock-free FIFO. Nothing is allocated or locked there.
 *
 *  pullSpectrum() runs on the message thread: it takes the newest fftSize
 *  samples, applies a Hann window, and smooths the dB magnitudes with a fast
 *  attack and a slower release so the display does not flicker.
 */
class SpectrumAnalyzer final
{
public:
    static constexpr int   fftOrder = 11;              // 2048-point FFT
    static constexpr int   fftSize  = 1 << fftOrder;
    static constexpr int   numBins  = fftSize / 2;
    static constexpr float floorDb  = -90.0f;

    SpectrumAnalyzer();

    /** Not for the audio thread: sizes the sample ring for the given rate. */
    void prepare (double sampleRate);

    /** Audio thread. Never blocks; samples are dropped if the reader falls behind. */
    void push (const juce::AudioBuffer<float>& buffer) noexcept;

    /** Message thread. Call periodically (for example from a juce::Timer). */
    void pullSpectrum();

    const std::array<float, numBins>& getMagnitudesDb() const noexcept { return magnitudesDb_; }
    double getSampleRate() const noexcept                                { return sampleRate_; }

private:
    double sampleRate_ = 44100.0;

    juce::AbstractFifo                 fifo_ { 4 * fftSize };
    std::vector<float>                 ring_;

    juce::dsp::FFT                     fft_ { fftOrder };
    juce::dsp::WindowingFunction<float> window_ { (size_t) fftSize, juce::dsp::WindowingFunction<float>::hann };
    std::array<float, 2 * fftSize>     fftData_ {};
    std::array<float, numBins>         magnitudesDb_ {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumAnalyzer)
};
