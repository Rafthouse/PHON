#include "PhMeter.h"

PhMeterBar::PhMeterBar()
{
    setOpaque (false);
}

void PhMeterBar::setLevel (float normalized) noexcept
{
    level_ = juce::jlimit (0.0f, 1.0f, normalized);
    repaint();
}

void PhMeterBar::paint (juce::Graphics& g)
{
    // FP-LATER: Draw the Rattosaurus meter:
    //   - Dark trough
    //   - Gradient fill: green → amber → red
    //   - Peak‑hold line
    juce::ignoreUnused (g);
}
