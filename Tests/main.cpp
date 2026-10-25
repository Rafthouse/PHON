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
    juce::ignoreUnused (argc, argv);

    PhTestRunner runner;
    runner.runAllTests();

    // JUCE 7 has no failure counter on UnitTestRunner; sum the per-test results instead.
    const auto results = runner.getNumResults();
    int fails = 0;
    for (int i = 0; i < results; ++i)
        fails += runner.getResult (i)->failures;

    // stdout, not the debug log, so CI and terminals show the summary.
    std::cout << "PHON tests complete — " << results << " test cases, " << fails << " failures." << std::endl;

    return fails > 0 ? 1 : 0;
}
