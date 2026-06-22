#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// =============================================================================
/** PHON editor.
 *
 *  Composes the custom UI components declared in Source/UI/.
 *  All attachment objects (SliderAttachment, ButtonAttachment, …) live here
 *  so they stay alive for the lifetime of the editor.
 */
class PhPluginEditor final : public juce::AudioProcessorEditor,
                             private juce::AudioProcessorValueTreeState::Listener
{
public:
    explicit PhPluginEditor (PhPluginProcessor&);
    ~PhPluginEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // ── Reference ─────────────────────────────────────────────────────────
    PhPluginProcessor& processor_;
    juce::AudioProcessorValueTreeState& apvts_;

    // ── Parameter attachments ─────────────────────────────────────────────
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  inputAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  outputAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  amountAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  phonAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  densityAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>  mixAttach_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> characterAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oversamplingAttach_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   safePeakAttach_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>   bypassAttach_;

    void parameterChanged (const juce::String&, float) override;

    // =======================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhPluginEditor)
};
