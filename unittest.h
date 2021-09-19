#if !defined(UNITTEST_H)
#define UNITTEST_H

#include <stddef.h>

typedef struct {
  const char *name;
  void (*test)(void);
  int failed_assertions;
  int failed_checks;
  void *state;
} unittest_test_t;

int UnittestEvaluate(const char *file, int line, int level, const char *expr,
                     int result, const char *msg);
void UnittestInit(int argc, char const *argv[]);
int UnittestRun(unittest_test_t tests[]);

#define UNITTEST_LEVEL_ASSERTION 1
#define UNITTEST_LEVEL_CHECK 2

#define UNITTEST_CHECK(x) \
  UnittestEvaluate(__FILE__, __LINE__, UNITTEST_LEVEL_CHECK, #x, x, NULL)
#define UNITTEST_CHECK_(x, msg) \
  UnittestEvaluate(__FILE__, __LINE__, UNITTEST_LEVEL_CHECK, #x, x, msg)
#define UNITTEST_ASSERT(x)                                                \
  if (UnittestEvaluate(__FILE__, __LINE__, UNITTEST_LEVEL_ASSERTION, #x, x, \
                       NULL) > 0)                                         \
    return;
#define UNITTEST_ASSERT_(x, msg)                                          \
  if (UnittestEvaluate(__FILE__, __LINE__, UNITTEST_LEVEL_ASSERTION, #x, x, \
                       msg) > 0)                                          \
    return;

#define UNITTEST_TEST(x) void x(void)

#define UNITTEST_TESTS unittest_test_t test_list[] 

#define UNITTEST_DECLARE(x) \
  { #x, x, 0, 0, NULL }

#define UNITTEST_END {NULL, NULL, 0, 0, NULL }


#if !defined(UNITTEST_DONT_USE_DEFAULT_MAIN)
extern unittest_test_t test_list[];
int main(int argc, char const *argv[])
{
    UnittestInit(argc, argv);
    return UnittestRun(test_list);
}
#endif // UNITTEST_USE_DEFAULT_MAIN





#endif  // UNITTEST_H
