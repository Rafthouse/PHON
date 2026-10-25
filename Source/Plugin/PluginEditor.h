#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PhDisplay.h"

// =============================================================================
/** PHON editor.
 *
 *  Composes the custom UI components declared in Source/UI/.
 *  All attachment objects (SliderAttachment, ButtonAttachment, …) live here
 *  so they stay alive for the lifetime of the editor.
 *
 *  A 30 Hz timer pulls the PRE / POST spectra from the processor and hands
 *  them to the display on the message thread.
 */
class PhPluginEditor final : public juce::AudioProcessorEditor,
                             private juce::AudioProcessorValueTreeState::Listener,
                             private juce::Timer
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

    // ── Display + spectrum switches ───────────────────────────────────────
    PhDisplay          display_;
    juce::TextButton   preButton_  { "PRE" };
    juce::TextButton   postButton_ { "POST" };

    void timerCallback() override;

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
