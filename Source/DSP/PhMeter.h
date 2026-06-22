#pragma once

#include <JuceHeader.h>

// =============================================================================
/** Perceptual level meter.
 *
 *  Computes RMS, peak, and a "PHON" virtual loudness metric derived from
 *  the ISO 226 contour model.  Used by the display panel and the UI meters.
 */
class PhMeter final
{
public:
    PhMeter() = default;
    ~PhMeter() = default;

    void prepare (double sampleRate);
    void reset() noexcept;

    /** Push one block through the metering engine. */
    void processBlock (const juce::AudioBuffer<float>& buffer);

    // ── Readouts (call from UI thread) ─────────────────────────────────
    float getRMSLevel() const noexcept    { return rms_; }
    float getPeakLevel() const noexcept   { return peak_; }
    float getPhonLevel() const noexcept   { return phonLevel_; }

private:
    float rms_       = 0.0f;
    float peak_      = 0.0f;
    float phonLevel_ = 0.0f;

    // FP-LATER: RMS window, peak hold, A‑weighting / ISO 226 filter

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhMeter)
};
