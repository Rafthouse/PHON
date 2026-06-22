#include "Harmonizer.h"

// =============================================================================
void Harmonizer::prepare (const juce::dsp::ProcessSpec& spec)
{
    // FP-LATER:
    //   - Allocate per‑band SVF / IIR filters
    //   - Pre‑compute wavetable for the selected character
    //   - Set up oversampling (resampler)
}

void Harmonizer::reset() noexcept
{
    // FP-LATER: Zero filter states, clear oversampler history.
}

// =============================================================================
void Harmonizer::processBlock (juce::AudioBuffer<float>& buffer)
{
    // FP-LATER:
    //   1. Oversample if needed (2× / 4× / 8×).
    //   2. For each band:
    //      a. Band‑pass the input.
    //      b. Generate harmonics according to CharacterMode (even, odd, etc.)
    //      c. Weight by phonStrength_ (ISO 226 correlation).
    //      d. Scale by amount_.
    //   3. Sum bands into the output buffer.
    //   4. Downsample if oversampled.
    juce::ignoreUnused (buffer);
}
