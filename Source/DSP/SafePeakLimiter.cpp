#include "SafePeakLimiter.h"

// =============================================================================
void SafePeakLimiter::prepare (const juce::dsp::ProcessSpec& spec)
{
    // FP-LATER:
    //   - Init lookahead delay (e.g. 2 ms).
    //   - Set up envelope follower time constants.
    //   - Reset gain.
    juce::ignoreUnused (spec);
}

void SafePeakLimiter::reset() noexcept
{
    // FP-LATER: clear delay, zero envelope, set gain = 1.0f.
}

// =============================================================================
void SafePeakLimiter::processBlock (juce::AudioBuffer<float>& buffer)
{
    // FP-LATER:
    //   1. Lookahead delay: read from delay line, write current sample.
    //   2. Envelope follower on lookahead sample.
    //   3. If envelope ≥ threshold → compute gain reduction.
    //   4. Apply gain with interpolated ramp to avoid clicks.
    //   5. Release smoothly.
    juce::ignoreUnused (buffer);
}
