#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Vertical meter strip (input / output).
 *
 *  Renders the gradient fill and peak‑hold line.
 */
class PhMeterBar final : public juce::Component
{
public:
    PhMeterBar();
    ~PhMeterBar() override = default;

    void setLevel (float normalized) noexcept;
    float getLevel() const noexcept { return level_; }

    void paint (juce::Graphics& g) override;

private:
    float level_ = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhMeterBar)
};
