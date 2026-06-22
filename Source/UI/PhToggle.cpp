#include "PhToggle.h"

PhToggle::PhToggle()
{
    setClickingTogglesState (true);
    setWantsKeyboardFocus (true);
}

void PhToggle::paint (juce::Graphics& g)
{
    // FP-LATER: Draw the Rattosaurus toggle:
    //   - Dark track with inset shadow
    //   - Amber‑lit knob when on
    juce::ignoreUnused (g);
}
