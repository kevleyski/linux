/*
 * Unit tests for mktime function
 * Tests time conversion functionality
 */

#include "../test_framework.h"
#include <time.h>

/* External function from kernel/mktime.c */
extern long kernel_mktime(struct tm * tm);

int main(void) {
    TEST_SUITE_BEGIN("MkTime Function");

    TEST_CASE_BEGIN("Unix epoch");
    struct tm tm = {0};
    tm.tm_year = 70;  // 1970
    tm.tm_mon = 0;    // January (0-11)
    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    long result = kernel_mktime(&tm);
    TEST_ASSERT_EQUAL(0, result, "Unix epoch (1970-01-01 00:00:00) = 0");

    TEST_CASE_BEGIN("Known timestamps");
    tm.tm_sec = 1;
    result = kernel_mktime(&tm);
    TEST_ASSERT_EQUAL(1, result, "One second after epoch");
    
    tm.tm_sec = 0;
    tm.tm_min = 1;
    result = kernel_mktime(&tm);
    TEST_ASSERT_EQUAL(60, result, "One minute after epoch");
    
    tm.tm_min = 0;
    tm.tm_hour = 1;
    result = kernel_mktime(&tm);
    TEST_ASSERT_EQUAL(3600, result, "One hour after epoch");

    TEST_CASE_BEGIN("Date calculations");
    tm.tm_hour = 0;
    tm.tm_mday = 2;
    result = kernel_mktime(&tm);
    TEST_ASSERT_EQUAL(86400, result, "One day after epoch (86400 seconds)");
    
    tm.tm_mday = 1;
    tm.tm_mon = 1;  // February
    result = kernel_mktime(&tm);
    TEST_ASSERT_EQUAL(2678400, result, "One month after epoch (31 days)");

    TEST_CASE_BEGIN("Year boundaries");
    tm.tm_mon = 0;   // January
    tm.tm_year = 71; // 1971
    result = kernel_mktime(&tm);
    TEST_ASSERT_EQUAL(31536000, result, "One year after epoch (365 days)");

    TEST_SUITE_END();
}

