#include "PhProcessor.h"

// =============================================================================
void PhProcessor::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate_ = spec.sampleRate;

    // FP-LATER: prepare() each DSP sub‑module:
    // harmonizer_.prepare   (spec);
    // densityShaper_.prepare (spec);
    // limiter_.prepare      (spec);
}

void PhProcessor::reset() noexcept
{
    // FP-LATER: reset() each DSP sub‑module:
    // harmonizer_.reset();
    // densityShaper_.reset();
    // limiter_.reset();
}

// =============================================================================
void PhProcessor::processBlock (juce::AudioBuffer<float>& buffer)
{
    if (params_.bypass)
        return;

    const auto numChannels = buffer.getNumChannels();
    const auto numSamples  = buffer.getNumSamples();

    // ── Input trim ─────────────────────────────────────────────────────
    // FP-LATER: Apply params_.input gain (dB → linear)

    // ── Harmonizer ─────────────────────────────────────────────────────
    // FP-LATER:
    //   harmonizer_.setAmount      (params_.amount);
    //   harmonizer_.setPhonStrength(params_.phon);
    //   harmonizer_.setCharacter   (params_.character);
    //   harmonizer_.setOversampling(params_.oversampling);
    //   harmonizer_.processBlock   (buffer);

    // ── Density Shaper ─────────────────────────────────────────────────
    // FP-LATER:
    //   densityShaper_.setDensity(params_.density);
    //   densityShaper_.processBlock(buffer);

    // ── Safe Peak Limiter ──────────────────────────────────────────────
    // FP-LATER:
    //   if (params_.safePeak)
    //       limiter_.processBlock(buffer);

    // ── Output trim ────────────────────────────────────────────────────
    // FP-LATER: Apply params_.output gain (dB → linear)

    // ── Dry/Wet mix ────────────────────────────────────────────────────
    // FP-LATER: Apply params_.mix via crossfade with a dry buffer copy.
}
