#pragma once

#include <JuceHeader.h>

// =============================================================================
/** True‑peak (or ISCL / ITU‑R BS.1770) limiter for output protection.
 *
 *  Acts as final stage before the output trim.  Designed to catch brief
 *  overshoots introduced by harmonic generation and density shaping.
 */
class SafePeakLimiter final
{
public:
    SafePeakLimiter() = default;
    ~SafePeakLimiter() = default;

    void prepare (const juce::dsp::ProcessSpec& spec);
    void reset() noexcept;

    void setThreshold (float dB) noexcept  { thresholdDB_ = dB; }

    void processBlock (juce::AudioBuffer<float>& buffer);

private:
    float thresholdDB_ = -0.3f;   // FP-LATER: make user‑adjustable

    // FP-LATER: lookahead delay line, envelope follower, gain computer
    // juce::dsp::DelayLine<float> delay_;
    // float gain_ = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SafePeakLimiter)
};
