/*
 * Unit tests for lib/string.c functions  
 * Tests kernel string implementations with inline assembly
 */

#include <stdio.h>
#include <stdlib.h>
#include "../test_framework.h"

/* Include the kernel's string header */
#include <linux/string.h>

int main(void) {
    TEST_SUITE_BEGIN("Lib String Functions");

    /* Test strcpy */
    TEST_CASE_BEGIN("strcpy");
    char dest[100];
    strcpy(dest, "hello");
    TEST_ASSERT_STR_EQUAL("hello", dest, "strcpy copies string correctly");
    strcpy(dest, "");
    TEST_ASSERT_STR_EQUAL("", dest, "strcpy handles empty string");

    /* Test strncpy */
    TEST_CASE_BEGIN("strncpy");
    char dest2[100] = {0};
    strncpy(dest2, "hello world", 5);
    dest2[5] = '\0'; /* Ensure null termination */
    TEST_ASSERT_STR_EQUAL("hello", dest2, "strncpy copies n characters");

    /* Test strcat */
    TEST_CASE_BEGIN("strcat");
    char dest3[100];
    strcpy(dest3, "Hello");
    strcat(dest3, " ");
    strcat(dest3, "World");
    TEST_ASSERT_STR_EQUAL("Hello World", dest3, "strcat concatenates strings");

    /* Test strcmp */
    TEST_CASE_BEGIN("strcmp");
    TEST_ASSERT_EQUAL(0, strcmp("test", "test"), "Equal strings return 0");
    TEST_ASSERT(strcmp("abc", "abd") < 0, "abc < abd");
    TEST_ASSERT(strcmp("abd", "abc") > 0, "abd > abc");

    /* Test strncmp */
    TEST_CASE_BEGIN("strncmp");
    TEST_ASSERT_EQUAL(0, strncmp("test", "test", 4), "Equal strings return 0");
    TEST_ASSERT_EQUAL(0, strncmp("test", "testing", 4), "First 4 chars match");
    TEST_ASSERT(strncmp("abc", "abd", 3) != 0, "Different within n chars");

    /* Test strlen */
    TEST_CASE_BEGIN("strlen");
    TEST_ASSERT_EQUAL(0, strlen(""), "Empty string has length 0");
    TEST_ASSERT_EQUAL(5, strlen("hello"), "String 'hello' has length 5");
    TEST_ASSERT_EQUAL(13, strlen("Hello, World!"), "String 'Hello, World!' has length 13");

    /* Test strchr */
    TEST_CASE_BEGIN("strchr");
    const char *str = "hello world";
    char *result = strchr(str, 'w');
    TEST_ASSERT_NOT_NULL(result, "strchr finds character");
    if (result) {
        TEST_ASSERT_STR_EQUAL("world", result, "strchr returns pointer to first occurrence");
    }
    result = strchr(str, 'z');
    TEST_ASSERT_NULL(result, "strchr returns NULL when character not found");

    /* Test strrchr */
    TEST_CASE_BEGIN("strrchr");
    result = strrchr(str, 'o');
    TEST_ASSERT_NOT_NULL(result, "strrchr finds character");
    if (result) {
        TEST_ASSERT_STR_EQUAL("orld", result, "strrchr returns pointer to last occurrence");
    }

    /* Test memset */
    TEST_CASE_BEGIN("memset");
    char buffer[10];
    memset(buffer, 'A', 10);
    TEST_ASSERT(buffer[0] == 'A' && buffer[9] == 'A', "memset fills memory");
    memset(buffer, 0, 10);
    int all_zero = 1;
    for (int i = 0; i < 10; i++) {
        if (buffer[i] != 0) all_zero = 0;
    }
    TEST_ASSERT(all_zero, "memset can zero memory");

    /* Test memcpy */
    TEST_CASE_BEGIN("memcpy");
    char src[] = "test data";
    char dst[20];
    memcpy(dst, src, strlen(src) + 1);
    TEST_ASSERT_STR_EQUAL(src, dst, "memcpy copies memory correctly");

    /* Test memcmp */
    TEST_CASE_BEGIN("memcmp");
    TEST_ASSERT_EQUAL(0, memcmp("test", "test", 4), "Equal memory returns 0");
    TEST_ASSERT(memcmp("abc", "abd", 3) != 0, "Different memory returns non-zero");

    /* Test memchr */
    TEST_CASE_BEGIN("memchr");
    const char *search = "hello world";
    char *found = (char *)memchr(search, 'w', strlen(search));
    TEST_ASSERT_NOT_NULL(found, "memchr finds character");
    if (found) {
        TEST_ASSERT(*found == 'w', "memchr returns correct pointer");
    }
    found = (char *)memchr(search, 'z', strlen(search));
    TEST_ASSERT_NULL(found, "memchr returns NULL when not found");

    /* Test strspn */
    TEST_CASE_BEGIN("strspn");
    size_t span = strspn("1234abc", "0123456789");
    TEST_ASSERT_EQUAL(4, span, "strspn counts matching characters");
    span = strspn("abcdef", "abc");
    TEST_ASSERT_EQUAL(3, span, "strspn stops at first non-match");

    /* Test strcspn */
    TEST_CASE_BEGIN("strcspn");
    span = strcspn("abcdef123", "0123456789");
    TEST_ASSERT_EQUAL(6, span, "strcspn counts non-matching characters");

    /* Test strpbrk */
    TEST_CASE_BEGIN("strpbrk");
    result = (char *)strpbrk("hello world", "aeiou");
    TEST_ASSERT_NOT_NULL(result, "strpbrk finds first character from set");
    if (result) {
        TEST_ASSERT(*result == 'e', "strpbrk returns correct character");
    }

    /* Test strstr */
    TEST_CASE_BEGIN("strstr");
    result = (char *)strstr("hello world", "wor");
    TEST_ASSERT_NOT_NULL(result, "strstr finds substring");
    if (result) {
        TEST_ASSERT_STR_EQUAL("world", result, "strstr returns pointer to substring");
    }
    result = (char *)strstr("hello world", "xyz");
    TEST_ASSERT_NULL(result, "strstr returns NULL when substring not found");

    TEST_SUITE_END();
}

