#include "PhKnob.h"

PhKnob::PhKnob()
{
    setSliderStyle (juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    setVelocityBasedMode (false);
    setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                         juce::MathConstants<float>::pi * 2.75f,
                         false);
}

void PhKnob::paint (juce::Graphics& g)
{
    // FP-LATER: Implement Rattosaurus knob rendering:
    //   - Background: radial gradient (metal look)
    //   - Arc: conic gradient (amber on dark)
    //   - Tick marker
    //   - Focus glow
    juce::ignoreUnused (g);
}

void PhKnob::resized()
{
    juce::Slider::resized();
}
