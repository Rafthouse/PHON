#pragma once

#include <JuceHeader.h>
#include "PhProcessor.h"

// =============================================================================
/** Harmonic generator core.
 *
 *  Produces perceptual harmonics correlated with the ISO 226 equal‑loudness
 *  contour (the "PHON" concept).  Six band‑limited harmonic engines each
 *  track a region of the audio spectrum and generate harmonics weighted by
 *  the chosen CharacterMode.
 */
class Harmonizer final
{
public:
    Harmonizer() = default;
    ~Harmonizer() = default;

    // ── Lifecycle ──────────────────────────────────────────────────────
    void prepare (const juce::dsp::ProcessSpec& spec);
    void reset() noexcept;

    // ── Parameters ─────────────────────────────────────────────────────
    void setAmount       (float amount) noexcept       { amount_       = amount; }
    void setPhonStrength (float strength) noexcept     { phonStrength_ = strength; }
    void setCharacter    (CharacterMode mode) noexcept { character_    = mode; }
    void setOversampling (OversamplingMode mode) noexcept { oversampling_ = mode; }

    // ── Processing ─────────────────────────────────────────────────────
    void processBlock (juce::AudioBuffer<float>& buffer);

private:
    float          amount_       = 0.0f;
    float          phonStrength_ = 0.0f;
    CharacterMode  character_    = CharacterMode::Clean;
    OversamplingMode oversampling_ = OversamplingMode::Off;

    // FP-LATER: Per‑band filter + harmonic oscillator state
    // struct Band { … };
    // std::vector<Band> bands_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Harmonizer)
};
