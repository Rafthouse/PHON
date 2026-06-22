#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Rattosaurus‑styled rotary knob.
 *
 *  Draws a retro‑futuristic knob face with:
 *    - Metal‑gradient body
 *    - Conic arc (value indicator)
 *    - Tick marker
 *    - Focus / hover glow
 *
 *  Uses the colour tokens from Design/tokens/colors.json.
 */
class PhKnob final : public juce::Slider
{
public:
    PhKnob();
    ~PhKnob() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    // FP-LATER: knob arc, shadow, highlight rendering

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhKnob)
};
