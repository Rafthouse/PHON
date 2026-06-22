#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Rattosaurus dropdown selector (Character, Oversampling).
 *
 *  Mimics the popup‑select HTML prototype:
 *    - LED indicator
 *    - Chevron
 *    - Styled dropdown menu
 */
class PhSelect final : public juce::ComboBox
{
public:
    PhSelect();
    ~PhSelect() override = default;

    void paint (juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhSelect)
};
