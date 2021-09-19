#define UNITTEST_DONT_USE_DEFAULT_MAIN
#include "unittest.h"

UNITTEST_TEST(CheckForAnswer)
{
    int answer = 41;
    UNITTEST_CHECK(answer == 42);
}
UNITTEST_TEST(CheckForAnswerWithCustomMessage)
{
    int answer = 41;
    UNITTEST_CHECK_(answer == 42, "The answer should be 42.");
}
UNITTEST_TEST(TheAnswerIsCorrect)
{
    int answer = 42;
    UNITTEST_ASSERT_(answer == 42, "The answer should be 42.");
}

UNITTEST_TESTS = {
    UNITTEST_DECLARE(TheAnswerIsCorrect),
    UNITTEST_DECLARE(CheckForAnswer),
    UNITTEST_DECLARE(CheckForAnswerWithCustomMessage),
    UNITTEST_END
};