#pragma once

#include <JuceHeader.h>
#include "PhProcessor.h"

// =============================================================================
/** PHON — Perceptual Harmonic Inflator
 *
 *  Plugin processor.  Owns the DSP chain and the AudioProcessorValueTreeState
 *  (APVTS) that exposes all parameters to the host.
 *
 *  Thread-safe notes:
 *    - Audio callbacks (prepareToPlay / processBlock / reset) may run on the
 *      real‑time audio thread.
 *    - Parameter changes arrive synchronously from the message thread; value
 *      smoothing is handled inside each DSP module.
 *    - APVTS is lock‑free for scalar parameters.
 */
class PhPluginProcessor final : public juce::AudioProcessor
{
public:
    // ── Lifecycle ────────────────────────────────────────────────────────
    PhPluginProcessor();
    ~PhPluginProcessor() override;

    // ── Audio ────────────────────────────────────────────────────────────
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // ── Editor ───────────────────────────────────────────────────────────
    bool hasEditor() const noexcept override          { return true; }
    juce::AudioProcessorEditor* createEditor() override;

    // ── Presets / state ──────────────────────────────────────────────────
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // ── Metadata ─────────────────────────────────────────────────────────
    const juce::String getName() const override       { return "PHON"; }
    bool acceptsMidi() const noexcept override         { return false; }
    bool producesMidi() const noexcept override        { return false; }
    double getTailLengthSeconds() const override       { return 0.0; }
    int getNumPrograms() override                      { return 1; }
    int getCurrentProgram() override                   { return 0; }
    void setCurrentProgram(int) override               {}
    const juce::String getProgramName(int) override    { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    // ── Access ───────────────────────────────────────────────────────────
    juce::AudioProcessorValueTreeState& getAPVTS() noexcept { return apvts_; }
    const PhProcessor& getProcessor() const noexcept        { return processor_; }

private:
    // ── Parameters ───────────────────────────────────────────────────────
    juce::AudioProcessorValueTreeState apvts_;

    // ── DSP ──────────────────────────────────────────────────────────────
    PhProcessor processor_;

    // =====================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhPluginProcessor)
};
