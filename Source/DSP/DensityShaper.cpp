#include "DensityShaper.h"

// =============================================================================
void DensityShaper::prepare (const juce::dsp::ProcessSpec& spec)
{
    // FP-LATER:
    //   - Init envelope follower with time constants derived from spec.
    //   - Set up RMS window size.
    juce::ignoreUnused (spec);
}

void DensityShaper::reset() noexcept
{
    // FP-LATER: envelope_ = 0.0f;
}

// =============================================================================
void DensityShaper::processBlock (juce::AudioBuffer<float>& buffer)
{
    // FP-LATER:
    //   1. Per‑sample envelope follower (peak or RMS).
    //   2. Gain computer:  g = f(density_, envelope, threshold).
    //   3. Apply gain (soft clip or dynamic reduction).
    //   4. Smooth gain changes with interpolated gain ramp.
    juce::ignoreUnused (buffer);
}
