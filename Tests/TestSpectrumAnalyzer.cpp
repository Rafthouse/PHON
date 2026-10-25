#include <JuceHeader.h>
#include "SpectrumAnalyzer.h"

// =============================================================================
class TestSpectrumAnalyzer final : public juce::UnitTest
{
public:
    TestSpectrumAnalyzer() : UnitTest ("SpectrumAnalyzer", "PHON") {}

    void runTest() override
    {
        beginTest ("1 kHz sine peaks at the matching FFT bin");

        constexpr double sampleRate = 48000.0;
        constexpr int blockSize = 512;
        constexpr int numBlocks = 8;   // 4096 samples, more than one FFT frame

        SpectrumAnalyzer analyzer;
        analyzer.prepare (sampleRate);

        juce::AudioBuffer<float> buffer (2, blockSize);

        for (int block = 0; block < numBlocks; ++block)
        {
            for (int i = 0; i < blockSize; ++i)
            {
                const auto t = (double) (block * blockSize + i) / sampleRate;
                const auto value = (float) std::sin (2.0 * juce::MathConstants<double>::pi * 1000.0 * t);

                buffer.setSample (0, i, value);
                buffer.setSample (1, i, value);
            }

            analyzer.push (buffer);
        }

        analyzer.pullSpectrum();

        const auto& magnitudes = analyzer.getMagnitudesDb();
        const auto peak = (int) std::distance (magnitudes.begin(),
                                               std::max_element (magnitudes.begin(), magnitudes.end()));
        const auto peakHz = peak * sampleRate / SpectrumAnalyzer::fftSize;

        expectWithinAbsoluteError (peakHz, 1000.0, sampleRate / SpectrumAnalyzer::fftSize);

        beginTest ("silence stays at the floor");

        SpectrumAnalyzer silent;
        silent.prepare (sampleRate);

        buffer.clear();
        for (int block = 0; block < numBlocks; ++block)
            silent.push (buffer);

        silent.pullSpectrum();

        const auto& floorMagnitudes = silent.getMagnitudesDb();
        const auto maxDb = *std::max_element (floorMagnitudes.begin(), floorMagnitudes.end());
        expect (maxDb <= SpectrumAnalyzer::floorDb + 1.0f, "silent input should not show energy");
    }
};

static TestSpectrumAnalyzer testSpectrumAnalyzer;
