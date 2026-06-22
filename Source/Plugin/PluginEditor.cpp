#include "PluginEditor.h"

// =============================================================================
PhPluginEditor::PhPluginEditor (PhPluginProcessor& processor)
    : AudioProcessorEditor (&processor)
    , processor_ (processor)
    , apvts_     (processor.getAPVTS())
{
    apvts_.addParameterListener ("input",        this);
    apvts_.addParameterListener ("output",       this);
    apvts_.addParameterListener ("amount",       this);
    apvts_.addParameterListener ("phon",         this);
    apvts_.addParameterListener ("density",      this);
    apvts_.addParameterListener ("mix",          this);
    apvts_.addParameterListener ("character",    this);
    apvts_.addParameterListener ("oversampling", this);
    apvts_.addParameterListener ("safePeak",     this);
    apvts_.addParameterListener ("bypass",       this);

    // FP-LATER: Instantiate custom UI components here (PhKnob, PhSelect, etc.)
    //           and attach them to apvts_ via SliderAttachment / ButtonAttachment.

    setSize (750, 500);
    setResizable (true, false);
    setResizeLimits (375, 250, 1500, 1000);
}

PhPluginEditor::~PhPluginEditor()
{
    apvts_.removeParameterListener ("input",        this);
    apvts_.removeParameterListener ("output",       this);
    apvts_.removeParameterListener ("amount",       this);
    apvts_.removeParameterListener ("phon",         this);
    apvts_.removeParameterListener ("density",      this);
    apvts_.removeParameterListener ("mix",          this);
    apvts_.removeParameterListener ("character",    this);
    apvts_.removeParameterListener ("oversampling", this);
    apvts_.removeParameterListener ("safePeak",     this);
    apvts_.removeParameterListener ("bypass",       this);
}

// =============================================================================
void PhPluginEditor::paint (juce::Graphics& g)
{
    // ── FP-LATER: Replace placeholder background with the Rattosaurus UI
    g.fillAll (juce::Colour (0xff0a070c));   // --bg

    g.setColour (juce::Colours::white.withAlpha (0.12f));
    g.setFont  (juce::Font (16.0f).withTypefaceStyle ("bold"));
    g.drawText ("PHON — Perceptual Harmonic Inflator",
                getLocalBounds(), juce::Justification::centred, false);

    g.setFont (juce::Font (12.0f));
    g.setColour (juce::Colour (0xff9a8aa6));
    g.drawText ("UI components will appear here once PhKnob / PhSelect / … are wired.",
                getLocalBounds().reduced (20), juce::Justification::centredBottom, false);
}

void PhPluginEditor::resized()
{
    // FP-LATER: Lay out UI child components per Design/tokens/layout.json
}

// =============================================================================
void PhPluginEditor::parameterChanged (const juce::String& paramID, float newValue)
{
    // FP-LATER: Push live parameter changes to the display/animation layer.
    juce::ignoreUnused (paramID, newValue);
}
