#include "PhDisplay.h"

namespace
{
    const juce::Colour kBackground { 0xff0d0910 };
    const juce::Colour kEdge       { 0xff7c4f96 };
    const juce::Colour kIvory      { 0xffd8c9a8 };   // PRE
    const juce::Colour kAmber      { 0xffe6a33c };   // POST
}

PhDisplay::PhDisplay()
{
    setOpaque (false);
    setInterceptsMouseClicks (false, false);

    preDb_.fill (SpectrumAnalyzer::floorDb);
    postDb_.fill (SpectrumAnalyzer::floorDb);
}

void PhDisplay::setSpectra (const std::array<float, SpectrumAnalyzer::numBins>& pre,
                            const std::array<float, SpectrumAnalyzer::numBins>& post,
                            double sampleRate)
{
    preDb_ = pre;
    postDb_ = post;
    sampleRate_ = sampleRate;
    repaint();
}

void PhDisplay::setShowPre (bool shouldShow)
{
    showPre_ = shouldShow;
    repaint();
}

void PhDisplay::setShowPost (bool shouldShow)
{
    showPost_ = shouldShow;
    repaint();
}

void PhDisplay::setHarmonicWeights (const std::array<float, 6>& weights) noexcept
{
    weights_ = weights;
    repaint();
}

// =============================================================================
void PhDisplay::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();

    g.setColour (kBackground);
    g.fillRoundedRectangle (bounds, 8.0f);
    g.setColour (kEdge.withAlpha (0.52f));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 8.0f, 1.0f);

    // Plot area sits below the header row (title + switches) and above the axis.
    const auto plot = bounds.reduced (12.0f, 0.0f).withTrimmedTop (34.0f).withTrimmedBottom (22.0f);

    const auto freqToX = [&] (double hz)
    {
        const auto t = std::log10 (hz / minHz) / std::log10 (maxHz / minHz);
        return plot.getX() + plot.getWidth() * (float) t;
    };

    const auto dbToY = [&] (float db)
    {
        const auto t = (db - SpectrumAnalyzer::floorDb) / -SpectrumAnalyzer::floorDb;
        return plot.getBottom() - plot.getHeight() * juce::jlimit (0.0f, 1.0f, t);
    };

    // ── Grid ──────────────────────────────────────────────────────────────
    g.setColour (kEdge.withAlpha (0.16f));

    for (const auto hz : { 20.0, 100.0, 1000.0, 10000.0 })
        g.drawVerticalLine (juce::roundToInt (freqToX (hz)), plot.getY(), plot.getBottom());

    for (float db = 0.0f; db >= SpectrumAnalyzer::floorDb; db -= 30.0f)
        g.drawHorizontalLine (juce::roundToInt (dbToY (db)), plot.getX(), plot.getRight());

    // ── Spectra ───────────────────────────────────────────────────────────
    const auto buildPath = [&] (const std::array<float, SpectrumAnalyzer::numBins>& magnitudesDb)
    {
        juce::Path path;
        bool started = false;

        for (int bin = 1; bin < SpectrumAnalyzer::numBins; ++bin)
        {
            const auto hz = bin * sampleRate_ / SpectrumAnalyzer::fftSize;

            if (hz < minHz || hz > maxHz)
                continue;

            const auto x = freqToX (hz);
            const auto y = dbToY (magnitudesDb[(size_t) bin]);

            if (! started)
            {
                path.startNewSubPath (x, y);
                started = true;
            }
            else
            {
                path.lineTo (x, y);
            }
        }

        return path;
    };

    if (showPost_)
    {
        const auto post = buildPath (postDb_);

        auto fill = post;
        fill.lineTo (plot.getRight(), plot.getBottom());
        fill.lineTo (plot.getX(), plot.getBottom());
        fill.closeSubPath();

        g.setColour (kAmber.withAlpha (0.12f));
        g.fillPath (fill);
        g.setColour (kAmber.withAlpha (0.95f));
        g.strokePath (post, juce::PathStrokeType (2.0f));
    }

    if (showPre_)
    {
        g.setColour (kIvory.withAlpha (0.55f));
        g.strokePath (buildPath (preDb_), juce::PathStrokeType (1.2f));
    }

    // ── Header title ──────────────────────────────────────────────────────
    g.setColour (juce::Colour (0xff9a8aa6));
    g.setFont (juce::Font (9.0f));
    g.drawText ("SPECTRUM", juce::Rectangle<float> (14.0f, 10.0f, 200.0f, 16.0f),
                juce::Justification::centredLeft, false);
}

void PhDisplay::resized() {}
