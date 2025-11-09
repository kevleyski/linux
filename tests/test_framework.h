/*
 * Simple Unit Testing Framework for Linux Kernel
 * 
 * This provides a lightweight testing framework that can run on macOS
 * without requiring the full kernel environment.
 */

#ifndef _TEST_FRAMEWORK_H
#define _TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>

/* Use standard string.h for tests, not kernel's */
#include <string.h>

/* Global test counters */
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Color codes for output */
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

/* Test assertion macros */
#define TEST_ASSERT(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "%s\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "%s\n", message); \
            printf("    Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) == (actual)) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "%s\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "%s (expected: %ld, got: %ld)\n", \
                   message, (long)(expected), (long)(actual)); \
            printf("    Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_NOT_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if ((expected) != (actual)) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "%s\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "%s (both values: %ld)\n", \
                   message, (long)(expected)); \
            printf("    Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_STR_EQUAL(expected, actual, message) \
    do { \
        tests_run++; \
        if (strcmp((expected), (actual)) == 0) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "%s\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "%s (expected: \"%s\", got: \"%s\")\n", \
                   message, (expected), (actual)); \
            printf("    Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_MEM_EQUAL(expected, actual, size, message) \
    do { \
        tests_run++; \
        if (memcmp((expected), (actual), (size)) == 0) { \
            tests_passed++; \
            printf(COLOR_GREEN "  ✓ " COLOR_RESET "%s\n", message); \
        } else { \
            tests_failed++; \
            printf(COLOR_RED "  ✗ " COLOR_RESET "%s (memory differs)\n", message); \
            printf("    Failed at %s:%d\n", __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_NULL(ptr, message) \
    TEST_ASSERT((ptr) == NULL, message)

#define TEST_ASSERT_NOT_NULL(ptr, message) \
    TEST_ASSERT((ptr) != NULL, message)

/* Test suite macros */
#define TEST_SUITE_BEGIN(name) \
    do { \
        printf("\n" COLOR_BLUE "══════════════════════════════════════════════════════\n" COLOR_RESET); \
        printf(COLOR_BLUE "  Test Suite: %s\n" COLOR_RESET, name); \
        printf(COLOR_BLUE "══════════════════════════════════════════════════════\n" COLOR_RESET); \
    } while(0)

#define TEST_CASE_BEGIN(name) \
    do { \
        printf("\n" COLOR_YELLOW "▶ Test Case: %s\n" COLOR_RESET, name); \
    } while(0)

#define TEST_SUITE_END() \
    do { \
        printf("\n" COLOR_BLUE "══════════════════════════════════════════════════════\n" COLOR_RESET); \
        printf(COLOR_BLUE "  Test Results\n" COLOR_RESET); \
        printf(COLOR_BLUE "══════════════════════════════════════════════════════\n" COLOR_RESET); \
        printf("  Total tests:  %d\n", tests_run); \
        printf(COLOR_GREEN "  Passed:       %d\n" COLOR_RESET, tests_passed); \
        if (tests_failed > 0) { \
            printf(COLOR_RED "  Failed:       %d\n" COLOR_RESET, tests_failed); \
        } else { \
            printf("  Failed:       %d\n", tests_failed); \
        } \
        printf(COLOR_BLUE "══════════════════════════════════════════════════════\n" COLOR_RESET); \
        return (tests_failed == 0) ? 0 : 1; \
    } while(0)

/* Helper to reset test counters between test files */
#define TEST_RESET_COUNTERS() \
    do { \
        tests_run = 0; \
        tests_passed = 0; \
        tests_failed = 0; \
    } while(0)

#endif /* _TEST_FRAMEWORK_H */

