#include "PhSelect.h"

PhSelect::PhSelect()
{
    setEditableText (false);
    setJustificationType (juce::Justification::centredLeft);
    setTextWhenNothingSelected (TRANS ("—"));
}

void PhSelect::paint (juce::Graphics& g)
{
    // FP-LATER: Draw the Rattosaurus select box:
    //   - Dark panel background
    //   - Purple border
    //   - LED dot (amber when active)
    //   - Drop‑down chevron
    juce::ignoreUnused (g);
}
