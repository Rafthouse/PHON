#include <JuceHeader.h>
#include "Harmonizer.h"

// =============================================================================
class TestHarmonizer final : public juce::UnitTest
{
public:
    TestHarmonizer() : UnitTest ("Harmonizer", "PHON") {}

    void runTest() override
    {
        beginTest ("prepare does not crash");
        Harmonizer h;
        juce::dsp::ProcessSpec spec { 44100.0, 512, 2 };
        h.prepare (spec);
        expect (true, "prepare completed");

        beginTest ("processBlock handles silence");
        juce::AudioBuffer<float> buf (2, 512);
        buf.clear();
        h.processBlock (buf);
        expect (true, "silence processed without error");

        beginTest ("reset after prepare");
        h.reset();
        expect (true, "reset completed");

        // FP-LATER: Add actual DSP unit tests:
        //   - Tone → harmonic content FFT analysis
        //   - Character mode output comparison
        //   - Oversampling signal integrity
        //   - Gain staging (amount → dBFS)
    }
};

static TestHarmonizer testHarmonizer;
