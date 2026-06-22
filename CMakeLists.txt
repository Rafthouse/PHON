cmake_minimum_required(VERSION 3.22)

# ── Project ──────────────────────────────────────────────────────────────────
project(PHON VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── JUCE ─────────────────────────────────────────────────────────────────────
# Download JUCE 7.0+ automatically if not found
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
    GIT_SHALLOW    TRUE
)
FetchContent_GetProperties(JUCE)
if(NOT juce_POPULATED)
    message(STATUS "Downloading JUCE 7.0.12 …")
    FetchContent_Populate(JUCE)
endif()
list(APPEND CMAKE_MODULE_PATH "${juce_SOURCE_DIR}/cmake")

find_package(JUCE CONFIG REQUIRED)
juce_add_plugin(PHON
    # ▸ Format support
    VST3_ONLY                    TRUE
    FORMATS_VST3                 TRUE
    FORMATS_STANDALONE           TRUE

    # ▸ Metadata
    COMPANY_NAME                 "Rattosaurus Lab"
    COMPANY_WEBSITE              "https://rattosaurus.lab"
    COMPANY_EMAIL                "mykyta@rattosaurus.lab"
    COMPANY_COPYRIGHT            "2026 Mykyta Shchur — Rattosaurus Lab"
    PRODUCT_NAME                 "PHON"
    PLUGIN_NAME                  "PHON"
    PLUGIN_MANUFACTURER_CODE     Ratt
    PLUGIN_CODE                  Phn0
    PLUGIN_DESCRIPTION           "Perceptual Harmonic Inflator"
    PLUGIN_IS_A_SYNTH           FALSE
    PLUGIN_IS_MIDI_EFFECT       FALSE
    VST3_CATEGORIES             "Audio FX"

    # ▸ Version
    VERSION                      "${PROJECT_VERSION}"
    VERSION_COMPATIBLE           "${PROJECT_VERSION}"
    VERSION_DESCRIPTION          "Perceptual Harmonic Inflator v${PROJECT_VERSION}"
)

# ── Sources ──────────────────────────────────────────────────────────────────
target_sources(PHON PRIVATE
    # ▸ Plugin entry
    Source/Plugin/PluginProcessor.cpp
    Source/Plugin/PluginProcessor.h
    Source/Plugin/PluginEditor.cpp
    Source/Plugin/PluginEditor.h

    # ▸ DSP
    Source/DSP/PhProcessor.cpp
    Source/DSP/PhProcessor.h
    Source/DSP/Harmonizer.cpp
    Source/DSP/Harmonizer.h
    Source/DSP/DensityShaper.cpp
    Source/DSP/DensityShaper.h
    Source/DSP/SafePeakLimiter.cpp
    Source/DSP/SafePeakLimiter.h
    Source/DSP/PhMeter.cpp
    Source/DSP/PhMeter.h

    # ▸ UI
    Source/UI/PhKnob.cpp
    Source/UI/PhKnob.h
    Source/UI/PhSelect.cpp
    Source/UI/PhSelect.h
    Source/UI/PhToggle.cpp
    Source/UI/PhToggle.h
    Source/UI/PhMeter.cpp
    Source/UI/PhMeter.h
    Source/UI/PhDisplay.cpp
    Source/UI/PhDisplay.h
)

# ── Include paths ────────────────────────────────────────────────────────────
target_include_directories(PHON PRIVATE
    Source/Plugin
    Source/DSP
    Source/UI
)

# ── Platform libs ────────────────────────────────────────────────────────────
if(WIN32)
    target_link_libraries(PHON PRIVATE winmm)
endif()

if(APPLE)
    target_link_libraries(PHON PRIVATE "-framework CoreAudio" "-framework CoreMIDI")
endif()

# ── Binary resources ─────────────────────────────────────────────────────────
juce_generate_juce_header(PHON)
juce_add_binary_data(PHON_BinaryData
    SOURCES
        Source/UI/Resources/images/logo-phon.svg
        Source/UI/Resources/images/knob-sprite.svg
    NAMESPACE
        PhBinaryData
)
target_link_libraries(PHON PRIVATE PHON_BinaryData)

# ── Tests ────────────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build the unit-test suite" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_executable(PHON_Tests
        Tests/main.cpp
        Tests/TestHarmonizer.cpp
        Tests/TestDensityShaper.cpp
    )
    target_include_directories(PHON_Tests PRIVATE Source/DSP)
    target_link_libraries(PHON_Tests PRIVATE juce::juce_recommended_test_helpers)
    add_test(NAME PHON_Tests COMMAND PHON_Tests)
endif()

# ── Compiler warnings ────────────────────────────────────────────────────────
if(MSVC)
    target_compile_options(PHON PRIVATE /W4 /permissive-)
else()
    target_compile_options(PHON PRIVATE -Wall -Wextra -Wpedantic -Wno-deprecated-declarations)
endif()
