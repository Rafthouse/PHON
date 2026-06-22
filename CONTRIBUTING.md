# Contributing to PHON

Thanks for your interest! Here's how to get started.

## Setup

1. Fork and clone the repo.
2. Make sure you have CMake ≥ 3.22 and a C++17 compiler.
3. Build:

```bash
cmake -B build
cmake --build build --parallel
```

## Code Style

- C++17, `.clang-format` enforced (WebKit style, 120 column limit).
- Names: `camelCase` for variables, `PascalCase` for classes, `UPPER_SNAKE` for macros.
- Smart pointers where ownership is clear; raw pointers for non‑owning observations.
- All DSP core classes must implement `prepare()`, `reset()`, and `processBlock()`.

## FP‑LATER Convention

Placeholder code is marked with `// FP-LATER:` followed by a short description.
This makes it searchable and helps track implementation progress.

## Testing

- Unit tests live in `/Tests` and use `juce::UnitTest`.
- Run with `ctest` or `--target PHON_Tests`.
- DSP tests should include:
  - Silence passthrough (no noise floor increase)
  - Tone → FFT analysis of harmonic output
  - Parameter sweep (no crashes)

## Pull Requests

1. Keep changes focused on one area (DSP / UI / Build).
2. Write tests for new DSP code.
3. Update `docs/ARCHITECTURE.md` if the pipeline changes.
4. Run `clang-format` before committing.

## Questions?

Open an issue or ping **@Grooveshaper**.
