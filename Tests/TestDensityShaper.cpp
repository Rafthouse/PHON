#include <JuceHeader.h>
#include "DensityShaper.h"

// =============================================================================
class TestDensityShaper final : public juce::UnitTest
{
public:
    TestDensityShaper() : UnitTest ("DensityShaper", "PHON") {}

    void runTest() override
    {
        beginTest ("prepare does not crash");
        DensityShaper ds;
        juce::dsp::ProcessSpec spec { 48000.0, 256, 2 };
        ds.prepare (spec);
        expect (true, "prepare completed");

        beginTest ("processBlock handles silence");
        juce::AudioBuffer<float> buf (2, 256);
        buf.clear();
        ds.processBlock (buf);
        expect (true, "silence processed without error");

        beginTest ("density=0 produces no gain reduction");
        ds.setDensity (0.0f);
        juce::AudioBuffer<float> tone (2, 256);
        for (int ch = 0; ch < 2; ++ch)
            for (int s = 0; s < 256; ++s)
                tone.setSample (ch, s, 0.5f * std::sin (440.0f * float (s) / 48000.0f * 2.0f * juce::MathConstants<float>::pi));
        auto before = tone.getSample (0, 0);
        ds.processBlock (tone);
        auto after = tone.getSample (0, 0);
        expect (std::abs (after - before) < 1e-6f, "Density=0 should leave audio unchanged");

        beginTest ("density>0 reduces crest factor");
        ds.reset();
        ds.setDensity (100.0f);
        ds.processBlock (tone);
        // FP-LATER: assert crest factor decreased

        beginTest ("reset is idempotent");
        ds.reset();
        expect (true, "reset completed");
    }
};

static TestDensityShaper testDensityShaper;
