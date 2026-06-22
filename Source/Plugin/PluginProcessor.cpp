#include "PluginProcessor.h"
#include "PluginEditor.h"

// =============================================================================
// Parameter layout
// =============================================================================
static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    using Param = juce::AudioParameterFloat;
    using Range = juce::NormalisableRange<float>;

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // ── IO ────────────────────────────────────────────────────────────
    layout.add(std::make_unique<Param>("input",    "Input",    Range{-12.0f, 12.0f, 0.1f},  0.0f));
    layout.add(std::make_unique<Param>("output",   "Output",   Range{-12.0f, 12.0f, 0.1f},  0.0f));

    // ── Core ──────────────────────────────────────────────────────────
    layout.add(std::make_unique<Param>("amount",   "Amount",   Range{  0.0f,100.0f, 0.5f}, 35.0f));
    layout.add(std::make_unique<Param>("phon",     "PHON",     Range{  0.0f,100.0f, 0.5f}, 50.0f));
    layout.add(std::make_unique<Param>("density",  "Density",  Range{  0.0f,100.0f, 0.5f}, 20.0f));
    layout.add(std::make_unique<Param>("mix",      "Mix",      Range{  0.0f,100.0f, 0.5f}, 100.0f));

    // ── Choices ───────────────────────────────────────────────────────
    const juce::StringArray chars { "CLEAN", "TUBE", "IRON", "DENSE" };
    const juce::StringArray os    { "OFF",   "2×",   "4×",   "8×" };

    layout.add(std::make_unique<juce::AudioParameterChoice>("character",    "Character",    chars, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("oversampling", "Oversampling", os,   0));

    // ── Bools ─────────────────────────────────────────────────────────
    layout.add(std::make_unique<juce::AudioParameterBool>("safePeak", "Safe Peak", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("bypass",   "Bypass",    false));

    return layout;
}

// =============================================================================
PhPluginProcessor::PhPluginProcessor()
    : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo())
                                        .withOutput ("Output", juce::AudioChannelSet::stereo()))
    , apvts_ (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

PhPluginProcessor::~PhPluginProcessor() = default;

// =============================================================================
void PhPluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec
    {
        sampleRate,
        static_cast<juce::uint32>(samplesPerBlock),
        static_cast<juce::uint32>(juce::jmax(getTotalNumInputChannels(), 1))
    };

    processor_.prepare(spec);
}

void PhPluginProcessor::releaseResources()
{
    processor_.reset();
}

void PhPluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // ── Pull current parameters ───────────────────────────────────────
    PhProcessor::Params params;
    params.input        = apvts_.getRawParameterValue("input")->load();
    params.output       = apvts_.getRawParameterValue("output")->load();
    params.amount       = apvts_.getRawParameterValue("amount")->load();
    params.phon         = apvts_.getRawParameterValue("phon")->load();
    params.density      = apvts_.getRawParameterValue("density")->load();
    params.mix          = apvts_.getRawParameterValue("mix")->load();
    params.character    = static_cast<CharacterMode>(static_cast<int>(apvts_.getRawParameterValue("character")->load()));
    params.oversampling = static_cast<OversamplingMode>(static_cast<int>(apvts_.getRawParameterValue("oversampling")->load()));
    params.safePeak     = apvts_.getRawParameterValue("safePeak")->load() > 0.5f;
    params.bypass       = apvts_.getRawParameterValue("bypass")->load() > 0.5f;

    processor_.setParams(params);

    // ── Process ───────────────────────────────────────────────────────
    juce::AudioBuffer<float> block(buffer.getArrayOfWritePointers(),
                                   buffer.getNumChannels(),
                                   buffer.getNumSamples());

    processor_.processBlock(block);
}

// =============================================================================
juce::AudioProcessorEditor* PhPluginProcessor::createEditor()
{
    return new PhPluginEditor (*this);
}

// =============================================================================
void PhPluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto state = apvts_.copyState(); ! state.isValid())
        juce::MemoryOutputStream(destData).writeString("{}");
    else
        juce::MemoryOutputStream(destData).writeString(state.toXmlString());
}

void PhPluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto xml = juce::XmlDocument::parse(juce::String::createStringFromData(data, sizeInBytes));
    if (xml != nullptr)
        apvts_.replaceState(juce::ValueTree::fromXml(*xml));
}

// =============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PhPluginProcessor();
}
