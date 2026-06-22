#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Main display panel — visualises the ISO 226 correlation and harmonic
 *  weight bars.
 *
 *  Renders with JUCE Graphics (no canvas API; uses Path + Image).
 */
class PhDisplay final : public juce::Component
{
public:
    PhDisplay();
    ~PhDisplay() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;

    /** Feed meter data from the DSP metering engine. */
    void setHarmonicWeights (const std::array<float, 6>& weights) noexcept;

private:
    std::array<float, 6> weights_ = {};

    // FP-LATER: phon readout, ISO 226 curve path

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhDisplay)
};
