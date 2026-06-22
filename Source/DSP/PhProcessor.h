#pragma once

#include <JuceHeader.h>

// ── Enums ────────────────────────────────────────────────────────────────────
enum class CharacterMode     { Clean = 0, Tube, Iron, Dense };
enum class OversamplingMode  { Off = 0, X2, X4, X8 };

// =============================================================================
/** Top‑level DSP engine for PHON.
 *
 *  Chains:
 *    Input trim → Harmonizer → DensityShaper → SafePeakLimiter → Output trim
 *
 *  All modules operate on a juce::dsp::ProcessSpec that may be oversampled
 *  (the oversampling rate is applied inside Harmonizer / DensityShaper).
 */
class PhProcessor final
{
public:
    // ── Parameter snapshot ─────────────────────────────────────────────
    struct Params
    {
        float           input        = 0.0f;
        float           output       = 0.0f;
        float           amount       = 35.0f;
        float           phon         = 50.0f;
        float           density      = 20.0f;
        float           mix          = 100.0f;
        CharacterMode   character    = CharacterMode::Clean;
        OversamplingMode oversampling = OversamplingMode::Off;
        bool            safePeak     = true;
        bool            bypass       = false;
    };

    // ── Lifecycle ──────────────────────────────────────────────────────
    PhProcessor() = default;
    ~PhProcessor() = default;

    void prepare (const juce::dsp::ProcessSpec& spec);
    void reset() noexcept;

    // ── Processing ─────────────────────────────────────────────────────
    void setParams (const Params& params) noexcept  { params_ = params; }
    void processBlock (juce::AudioBuffer<float>& buffer);

    // ── Access ─────────────────────────────────────────────────────────
    const Params& getParams() const noexcept { return params_; }

private:
    Params params_;

    // FP-LATER: member DSP modules (Harmonizer, DensityShaper, etc.)
    // Harmonizer   harmonizer_;
    // DensityShaper densityShaper_;
    // SafePeakLimiter limiter_;

    double sampleRate_ = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhProcessor)
};
