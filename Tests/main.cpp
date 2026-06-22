/** PHON Unit‑Test entry point.
 *
 *  Compiled with BUILD_TESTS=ON.  Links against juce::juce_recommended_test_helpers
 *  which provides JUCE's own UnitTestRunner.
 */
#include <JuceHeader.h>

class PhTestRunner final : public juce::UnitTestRunner
{
public:
    PhTestRunner()
    {
        setAssertOnFailure (false);
    }
};

// =============================================================================
int main (int argc, char* argv[])
{
    PhTestRunner runner;
    runner.runAllTests();

    const auto results = runner.getNumResults();
    const auto fails   = runner.getNumFailures();

    juce::Logger::outputDebugString (
        juce::String ("PHON tests complete — ")
        + juce::String (results) + " tests, "
        + juce::String (fails)   + " failures.");

    return fails > 0 ? 1 : 0;
}
