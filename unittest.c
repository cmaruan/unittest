#include "unittest.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int failed_assertions;
int failed_checks;
int verbosity = 1;
unittest_test_t *current_test;



void ResetState() {
    failed_assertions = 0;
    failed_checks = 0;
    current_test = NULL;
}


void UnittestInit(int argc, char const *argv[])
{

}

void ShowTestInfoShort(const unittest_test_t *test)
{
    static const char padding[] = \
        ".....................................................................";
    int pad_length = sizeof(padding) - strlen(test->name);
    if (pad_length < 0)
        pad_length = 0;
    fprintf(stderr, "%s %*.*s", test->name, pad_length, pad_length, padding);
    if (test->failed_assertions > 0 || test->failed_checks > 0) {
        fprintf(stderr, " FAILED\n");
    } else {
        fprintf(stderr, " OK\n");
    }
}

int RunTest(unittest_test_t *test) {
    ResetState();
    current_test = test;
    test->test();
    test->failed_assertions = failed_assertions;
    test->failed_checks = failed_checks;
    if (verbosity > 0)
        ShowTestInfoShort(test);
    return failed_assertions > 0 || failed_checks > 0;
}

int UnittestRun(unittest_test_t tests[])
{
    int i;
    int failed = 0;
    unittest_test_t *test = tests;
    while (test->test != NULL) {
        failed += RunTest(test++);
    }
    test = tests;
    while (test->test != NULL) {
        if (test->failed_assertions > 0 || test->failed_checks > 0) {
            fprintf(stderr, "-----------------------------------------------------------------------\n");
            fprintf(stderr, "Test `%s` FAILED\n", test->name);
            fprintf(stderr, "%s\n", (const char*)test->state);
        }
        free(test->state);
        test++;
    }
    fprintf(stderr, "-----------------------------------------------------------------------\n");

    return failed;
}

char * AppendString(char *a, const char *b)
{
    size_t sz_a, sz_b;
    int offset = 0;
    sz_b = strlen(b);
    if (a == NULL) {
        sz_a = 0;
    } else {
        sz_a = strlen(a);
    }
    char *str = malloc(sz_a + sz_b + 2);
    
    if (sz_a != 0)
        offset += sprintf(str + offset, "%s\n", a);
    
    if (sz_b != 0)
        offset += sprintf(str + offset, "%s", b);
    
    if (a != NULL)
        free(a);

    return str;
}

int UnittestEvaluate(const char *file, int line, int level, const char *expr, int result, const char *msg)
{
    static char buff[4096];
    int sz = 0;
    int offset = 0;
    const char *level_name;
    if (!result) {
        sz = sprintf(buff + sz, "[%s:%d] ", file, line);
        offset = sz;
        if (level == UNITTEST_LEVEL_ASSERTION) {
            failed_assertions++;
            sz += sprintf(buff + sz, "ASSERTION FAILED: ");
        } else if (level == UNITTEST_LEVEL_CHECK) {
            failed_checks++;
            sz += sprintf(buff + sz, "CHECK FAILED: ");
        }
        
        sz += sprintf(buff + sz, "`%s` is false.", expr);
        if (msg) 
            sz += sprintf(buff + sz - 1, "\n%*.*s%s", offset, offset, "", msg);
        
        current_test->state = AppendString((char*)current_test->state, buff);
    }
    return failed_assertions > 0 || failed_checks > 0;
}

