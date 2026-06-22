#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Adaptive density shaper — reduces crest factor while preserving
 *  perceived loudness.
 *
 *  Acts as a soft‑knee compressor / clipper with attack/release shaped
 *  by the input's own dynamics (self‑modulating).
 */
class DensityShaper final
{
public:
    DensityShaper() = default;
    ~DensityShaper() = default;

    void prepare (const juce::dsp::ProcessSpec& spec);
    void reset() noexcept;

    void setDensity (float density) noexcept  { density_ = density; }

    void processBlock (juce::AudioBuffer<float>& buffer);

private:
    float density_ = 0.0f;

    // FP-LATER: envelope follower, RMS tracker, gain computer state
    // float envelope_ = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DensityShaper)
};
