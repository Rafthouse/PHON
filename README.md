# PHON — Perceptual Harmonic Inflator

<img src="Design/Prototypes/index.html" alt="UI Prototype" width="100%" />

**PHON** is an audio plugin (VST3 / AU) that inflates harmonic content by correlating generated harmonics with the ISO 226 equal‑loudness contour — the psychacoustic model behind the *phon* scale.

Built with **JUCE 7** + **CMake**, targeting **Windows**, **macOS**, and **Linux**.

---

## Features

- **Perceptual harmonic generation** — 6 band‑limited harmonic engines weighted by the ISO 226 curve
- **Character modes** — CLEAN, TUBE, IRON, DENSE
- **Adaptive density shaper** — reduces crest factor while preserving loudness
- **Safe Peak limiter** — true‑peak output protection
- **Oversampling** — 2× / 4× / 8× internal resampling
- **Custom Rattosaurus UI** — retro‑futuristic design with animated display

## Quick Start

### Prerequisites

- CMake ≥ 3.22
- C++17 compiler (MSVC 2022, Clang 15+, GCC 11+)
- JUCE 7.0.12+ (downloaded automatically by CMake)

### Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

The VST3 binary will be in `build/PHON_artefacts/Release/VST3/`.

### Build Tests

```bash
cmake -B build -DBUILD_TESTS=ON
cmake --build build --parallel --target PHON_Tests
ctest --test-dir build
```

## Project Structure

```
PHON/
├── .github/workflows/   # CI (Linux / macOS / Windows)
├── Design/              # UI prototypes, SVG, design tokens
│   ├── Prototypes/      # HTML/CSS/JS prototype (v4)
│   ├── SVG/             # Vector assets
│   ├── tokens/          # Color, typography, layout JSON
│   └── wireframes/
├── Source/
│   ├── Plugin/          # JUCE processor + editor
│   ├── DSP/             # Harmonizer, DensityShaper, Limiter, Meter
│   └── UI/              # PhKnob, PhSelect, PhToggle, PhDisplay, PhMeter
├── Tests/               # Catch2-style JUCE unit tests
├── docs/                # Architecture documentation
├── CMakeLists.txt
├── .clang-format
└── LICENSE (MIT)
```

## DSP Pipeline

```
Input → Input Trim → Harmonizer → Density Shaper → Safe Peak Limiter → Output Trim → Dry/Wet Mix → Output
                         │
                    ISO 226 Weight
                    (PHON knob)
```

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT — see [LICENSE](LICENSE).

---

*Handcrafted in Ukraine by Mykyta Shchur — Rattosaurus Lab*
