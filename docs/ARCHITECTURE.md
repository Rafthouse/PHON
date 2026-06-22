# PHON Architecture

## Overview

PHON is a stereo audio plugin (VST3/AU) built on JUCE 7. Its core innovation is a harmonic generator whose output is weighted by the ISO 226 equal‑loudness contour — the psychacoustic model behind the *phon* scale.

## DSP Pipeline

```
                     ┌──────────────────────────────────────┐
 Input ──→ Trim ──→ │           Harmonizer                  │
                     │  ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐ │
                     │  │Band 1│ │Band 2│ │ …   │ │Band 6│ │
                     │  └──────┘ └──────┘ └──────┘ └──────┘ │
                     │         │  ISO 226 weight              │
                     │         │  (PHON knob)                │
                     └─────────┼────────────────────────────┘
                               │
                               ▼
                     ┌──────────────────┐
                     │ Density Shaper   │
                     │ (crest reduction)│
                     └────────┬─────────┘
                              │
                     ┌──────────────────┐
                     │ Safe Peak Limit  │
                     │ (true‑peak clamp)│
                     └────────┬─────────┘
                              │
 Output ←── Trim ←── Dry/Wet Mix ←──┘
```

### Modules

| Module | File | Responsibility |
|--------|------|----------------|
| `PhProcessor` | `Source/DSP/PhProcessor.h/.cpp` | Top‑level DSP chain, parameter routing |
| `Harmonizer` | `Source/DSP/Harmonizer.h/.cpp` | 6‑band perceptual harmonic generator |
| `DensityShaper` | `Source/DSP/DensityShaper.h/.cpp` | Self‑modulating crest factor reducer |
| `SafePeakLimiter` | `Source/DSP/SafePeakLimiter.h/.cpp` | Lookahead true‑peak limiter |
| `PhMeter` | `Source/DSP/PhMeter.h/.cpp` | RMS / peak / PHON loudness metering |

## Parameter Architecture

Parameters are managed by `juce::AudioProcessorValueTreeState` (lock‑free, thread‑safe).
Each audio‑rate parameter is read from `std::atomic<float>` in the audio thread via `getRawParameterValue`.

See `PluginProcessor.cpp` → `createParameterLayout()` for the full parameter list.

## UI Architecture

The Rattosaurus UI is composed of custom JUCE components:

| Component | File | Prototype equivalent |
|-----------|------|---------------------|
| `PhKnob` | `Source/UI/PhKnob` | `.knob` rotary with arc |
| `PhSelect` | `Source/UI/PhSelect` | `.popup-select` dropdown |
| `PhToggle` | `Source/UI/PhToggle` | `.toggle` switch |
| `PhMeterBar` | `Source/UI/PhMeter` | `.meter` vertical strip |
| `PhDisplay` | `Source/UI/PhDisplay` | `#scope` canvas area |

Colour, typography, and layout tokens are defined in `Design/tokens/` as JSON.

## Oversampling

When oversampling is enabled (`2×` / `4×` / `8×`), the Harmonizer runs at an elevated sample rate while the rest of the chain stays at the native rate. This is implemented via JUCE's `dsp::Oversampling` or a custom resampler.

## Build System

CMake with `FetchContent` downloads JUCE automatically.
GitHub Actions builds for all three platforms on every push.
Unit tests use JUCE's built‑in `UnitTest` framework.

## Roadmap

- [ ] Harmonizer DSP implementation
- [ ] DensityShaper implementation
- [ ] SafePeakLimiter implementation
- [ ] Custom UI paint (all components)
- [ ] Oversampling integration
- [ ] Preset management
- [ ] AAX format support
