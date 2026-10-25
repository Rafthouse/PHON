#pragma once

#include <JuceHeader.h>
#include "SpectrumAnalyzer.h"

// =============================================================================
/** Main display panel — shows the PRE (dry input) and POST (after the chain)
 *  spectra on a log-frequency axis, each with its own on/off switch.
 *
 *  Renders with JUCE Graphics (Path + stroke); no image assets.
 */
class PhDisplay final : public juce::Component
{
public:
    PhDisplay();
    ~PhDisplay() override = default;

    void paint (juce::Graphics& g) override;
    void resized() override;

    /** Feed both spectra (dB per FFT bin) from the message thread. */
    void setSpectra (const std::array<float, SpectrumAnalyzer::numBins>& pre,
                     const std::array<float, SpectrumAnalyzer::numBins>& post,
                     double sampleRate);

    void setShowPre  (bool shouldShow);
    void setShowPost (bool shouldShow);

    /** Feed meter data from the DSP metering engine. */
    void setHarmonicWeights (const std::array<float, 6>& weights) noexcept;

private:
    static constexpr float minHz = 20.0f;
    static constexpr float maxHz = 20000.0f;

    std::array<float, SpectrumAnalyzer::numBins> preDb_;
    std::array<float, SpectrumAnalyzer::numBins> postDb_;
    double sampleRate_ = 44100.0;
    bool showPre_  = true;
    bool showPost_ = true;

    std::array<float, 6> weights_ = {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhDisplay)
};
