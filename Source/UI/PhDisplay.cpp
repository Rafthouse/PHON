#include "PhDisplay.h"

PhDisplay::PhDisplay()
{
    setOpaque (false);
    setInterceptsMouseClicks (false, false);
}

void PhDisplay::setHarmonicWeights (const std::array<float, 6>& weights) noexcept
{
    weights_ = weights;
    repaint();
}

void PhDisplay::paint (juce::Graphics& g)
{
    // FP-LATER:
    //   1. Draw grid (12 vertical + 6 horizontal, faint purple).
    //   2. Draw ISO 226 curve as a Path.
    //   3. Draw six harmonic weight bars.
    //   4. Draw PHON readout in Share Tech Mono.
    juce::ignoreUnused (g);
}

void PhDisplay::resized() {}
