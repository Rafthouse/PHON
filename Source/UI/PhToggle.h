#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Rattosaurus toggle switch (Safe Peak, Bypass).
 */
class PhToggle final : public juce::ToggleButton
{
public:
    PhToggle();
    ~PhToggle() override = default;

    void paint (juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhToggle)
};
