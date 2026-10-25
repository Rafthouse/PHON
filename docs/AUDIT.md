# PHON — Code & Design Audit

Scope: repository state at `master` (commit `f93fea5`), plus the local fixes listed in §4.
Method: full read of `Source/`, `Tests/`, `CMakeLists.txt`, CI workflow, `Design/` and `docs/`;
a clean CMake configure (`-DBUILD_TESTS=ON`) run on this machine. **No compile or test run** — no C++
compiler is available in this environment.

---

## 1. Summary

PHON is at the **scaffold stage**. The project structure, parameter layout and UI prototype are in place,
but the audio path does nothing: every DSP module is a stub (`// FP-LATER`), so the plugin is a passthrough
and every parameter is inert. The README describes features (harmonic generation, density shaper,
limiter, oversampling) that are not implemented.

The build was also broken on a clean checkout. That is fixed locally (§4).

| Area | State |
|---|---|
| DSP (Harmonizer, DensityShaper, Limiter, Meter, PhProcessor) | Stubs. `processBlock` bodies are empty. |
| Parameters / state | Declared and persisted (APVTS); none reach the DSP. |
| Editor / UI components | Empty `paint()` in every `Ph*` component; editor shows a placeholder label. |
| Build (CMake) | Failed to configure on clean checkout. Fixed locally. |
| Tests | Vacuous (`expect(true, …)`); test binary did not link. Fixed linking, assertions still missing. |
| CI | Triggered on `main`/`develop`; repo default branch is `master`, so pushes/PRs never matched. Fixed. |
| Design prototype (`Design/Prototypes/index.html`) | Strong visual direction. Small a11y/contrast/copy issues. Partly fixed. |

---

## 2. Findings — code

### Critical

1. **No audio processing.** `Source/DSP/*.cpp` bodies are `// FP-LATER` comments plus `ignoreUnused`.
   `PhProcessor::processBlock` (`Source/DSP/PhProcessor.cpp:23`) reads params but never calls a module,
   so the plugin is a bit-exact passthrough at every setting. Anyone installing the VST3 gets no effect.
2. **Parameters are not wired to DSP.** `PhPluginProcessor::processBlock`
   (`Source/Plugin/PluginProcessor.cpp:66`) pulls all ten parameters into `PhProcessor::Params`, but
   `input`/`output` (dB) are never applied, and `character`, `oversampling`, `safePeak`, `mix` are unused.
   The dry/wet `mix` is also unimplemented.
3. **Editor has no controls.** `PhPluginEditor` attaches no `SliderAttachment`/`ButtonAttachment` even
   though the members exist (`PluginEditor.h:30–40`). Users cannot change a parameter from the UI.

### High

4. **Build did not configure on a clean checkout** (`CMakeLists.txt`). `FetchContent_Populate(JUCE)` only
   downloads the source; `find_package(JUCE CONFIG REQUIRED)` then fails because JUCE's package file exists
   only in its own build tree. Reproduced on CMake 4.3.3 (`Could not find a package configuration file provided by "JUCE"`).
   **Fixed:** `FetchContent_MakeAvailable(JUCE)`. Configure now succeeds.
5. **Test target did not link.** `PHON_Tests` compiled `Tests/TestHarmonizer.cpp` / `TestDensityShaper.cpp`
   but not `Source/DSP/Harmonizer.cpp` / `DensityShaper.cpp`, so `Harmonizer::prepare` etc. were unresolved.
   It also linked `juce::juce_recommended_test_helpers`, which I could not confirm is a JUCE 7 target, and
   `#include <JuceHeader.h>` requires a generated header that a plain `add_executable` does not get.
   **Fixed:** the test target is now `juce_add_console_app`, links `juce_core` + `juce_dsp`, and compiles the two DSP sources.
   *Not compiled — verify with a real build.*
6. **CI never ran on this repo's branches.** Workflow triggers were `main`/`develop`; the default branch is `master`.
   **Fixed:** triggers now `master`/`develop`.
7. **CI test step pointed at the wrong path.** `./build/Tests/PHON_Tests` — the root `add_executable` writes to `build/`.
   **Fixed:** `./build/PHON_Tests`.

### Medium

8. **Tests assert nothing.** Every case is `expect(true, …)`. `density>0 reduces crest factor` has a
   `// FP-LATER` where its assertion should be. CONTRIBUTING.md requires silence passthrough, FFT
   harmonic analysis and parameter sweeps — none exist yet.
9. **Placeholder metadata in the build.** `COMPANY_WEBSITE "https://rattosaurus.lab"`,
   `COMPANY_EMAIL "mykyta@rattosaurus.lab"`, and the `$schema` URLs in `Design/tokens/*.json` use a
   `.lab` domain that is not a verified site. Plugin metadata ships into the VST3 bundle.
10. **Bundle ID has spaces.** CMake warns: `JUCE_BUNDLE_ID 'com.Rattosaurus Lab.PHON'`. Set `BUNDLE_ID`
   to a valid reverse-DNS id — **needs your decision**, since the AU/VST3 identity is hard to change after release.
11. **Per-block string lookups in the audio thread.** `apvts_.getRawParameterValue("…")` is called ten times
    per `processBlock`. Cache the `std::atomic<float>*` pointers in the constructor.
12. **Unneeded buffer wrapper.** `PluginProcessor.cpp:86` builds a non-owning `AudioBuffer` from the
    write pointers; `processor_.processBlock(buffer)` takes the same buffer directly.
13. **Duplicated prototype.** `docs/index.html` is a byte-identical copy of `Design/Prototypes/index.html`
    (kept for GitHub Pages). Two files will drift. Options: generate `docs/` in CI, or drop the `Design/` copy.
14. **Stale README.** It claims features that do not exist yet (see §1) and the roadmap in
    `docs/ARCHITECTURE.md` is the honest version. Align the README "Features" list with the roadmap.
15. **`getStateInformation` writes `"{}"` when the state is invalid** (`PluginProcessor.cpp:102`). This is not XML,
    so `setStateInformation` silently ignores it. Harmless now; write a proper empty XML state instead.

### Low / to verify

16. `VST3_ONLY TRUE` combined with `FORMATS_VST3`/`FORMATS_STANDALONE` in `juce_add_plugin` looks contradictory — check the intended formats.
17. `juce_add_plugin` is given `PLUGIN_IS_A_SYNTH FALSE` etc. — fine; `Tests/main.cpp` has unused `argc/argv` (warning under `/W4`).
18. The `FetchContent` GIT_TAG is pinned to `7.0.12`, which matches CI. Good.
19. Editor `setResizeLimits` min 375×250 — at that size the 750×500 layout will clip. Scale the content instead of resizing the grid.

---

## 3. Findings — design (`Design/Prototypes/index.html`)

What works: a coherent visual system (purple trim, amber active state, brass labels), a clear hero control
(PHON), a live ISO 226 scope, and keyboard/ARIA handling on knobs and selects.

| # | Finding | Status |
|---|---|---|
| D1 | Footer read "RATOSAURUS" — brand is "Rattosaurus". | **Fixed** |
| D2 | Footer and scope footnotes at ~3.4:1 contrast on the background (WCAG AA needs 4.5:1 for small text). Footer also 7px. | **Fixed** — colours lifted to `#8a7a98` / `#8f7f9c`, footer 8px. |
| D3 | Knobs announced as `amount`, `phon` — raw ids, not labels. | **Fixed** — `aria-label` uses the visible caption; `aria-valuetext` includes the unit. |
| D4 | Safe Peak switch was a click-only `div` with no role. | **Fixed** — `role="switch"`, `aria-checked`, Enter/Space. |
| D5 | Footer said "UI PROTOTYPE 04" while the title says v4. | **Fixed** — "V4". |
| D6 | Footer signature reads **"By Mykyta Shchurr"**; README and CMake say **"Mykyta Shchur"**. | **Needs your decision** — which spelling is correct? Not changed. |
| D7 | Smallest labels are 8–9px (serial, meter scale, sub-values). On a plugin window that is scaled down with `--ui-scale`, these become unreadable. | Open — set 10px as the floor for anything that carries information. |
| D8 | The `Design/tokens/*.json` schema URLs point to `rattosaurus.lab`. | Open — see finding 9. |
| D9 | The native editor (`Source/UI`) does not implement any of this design yet — every `paint()` is empty. The prototype is the only visual reference. | Open — largest design gap. |

---

## 4. Changes made locally (not committed)

- `CMakeLists.txt` — `FetchContent_MakeAvailable(JUCE)`; test target as `juce_add_console_app` with DSP sources and `juce_core`/`juce_dsp`.
- `.github/workflows/build.yml` — branches `master`/`develop`; test binary path.
- `README.md` — removed the broken `<img src="…html">`; link to the prototype instead.
- `Design/Prototypes/index.html` and `docs/index.html` (identical) — D1–D5.
- `docs/AUDIT.md` — this file.

Verified: CMake configure succeeds (`-DBUILD_TESTS=ON`), prototype inline script parses.
**Not verified:** compile, link, test run, CI run, visual rendering of the prototype edits.

---

## 5. Recommended order

1. Confirm the spelling (D6) and the bundle ID (finding 10).
2. Build on macOS/Windows/Linux (or push and let CI run) to verify §4 changes.
3. Implement the DSP stubs in the order of the signal chain, with tests written first
   (silence passthrough, harmonic FFT, parameter sweep) — the test scaffolding exists.
4. Wire the parameters, then the editor components (`PhKnob`, `PhSelect`, `PhToggle`, `PhMeter`, `PhDisplay`)
   against `Design/tokens/*.json`.
5. Replace the duplicated `docs/index.html` with a CI copy step.
