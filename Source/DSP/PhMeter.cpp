#include "PhMeter.h"

// =============================================================================
void PhMeter::prepare (double sampleRate)
{
    // FP-LATER:
    //   - Set RMS window length (e.g. 50 ms).
    //   - Allocate ring buffer for RMS computation.
    //   - Initialise peak‑hold timer.
    juce::ignoreUnused (sampleRate);
}

void PhMeter::reset() noexcept
{
    rms_       = 0.0f;
    peak_      = 0.0f;
    phonLevel_ = 0.0f;
}

// =============================================================================
void PhMeter::processBlock (const juce::AudioBuffer<float>& buffer)
{
    // FP-LATER:
    //   1. Per‑channel RMS.
    //   2. Per‑channel peak.
    //   3. Weighted PHON loudness via ISO 226 "A‑curve" approximation.
    juce::ignoreUnused (buffer);
}
