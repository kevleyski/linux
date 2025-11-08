/*
 * Unit tests for kernel vsprintf function
 * Tests printf-style formatting
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../test_framework.h"

/* External vsprintf from kernel */
extern int vsprintf(char *buf, const char *fmt, va_list args);

/* Wrapper to test sprintf functionality */
static int test_sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    int result;
    
    va_start(args, fmt);
    result = vsprintf(buf, fmt, args);
    va_end(args);
    
    return result;
}

int main(void) {
    char buffer[256];
    
    TEST_SUITE_BEGIN("VSprintf/Sprintf Functions");

    /* Test basic string formatting */
    TEST_CASE_BEGIN("String formatting");
    test_sprintf(buffer, "Hello, %s!", "World");
    TEST_ASSERT_STR_EQUAL("Hello, World!", buffer, "String substitution works");
    
    test_sprintf(buffer, "%s %s %s", "one", "two", "three");
    TEST_ASSERT_STR_EQUAL("one two three", buffer, "Multiple string substitutions");

    /* Test integer formatting */
    TEST_CASE_BEGIN("Integer formatting");
    test_sprintf(buffer, "Value: %d", 42);
    TEST_ASSERT_STR_EQUAL("Value: 42", buffer, "Decimal integer formatting");
    
    test_sprintf(buffer, "Negative: %d", -123);
    TEST_ASSERT_STR_EQUAL("Negative: -123", buffer, "Negative integer formatting");
    
    test_sprintf(buffer, "Zero: %d", 0);
    TEST_ASSERT_STR_EQUAL("Zero: 0", buffer, "Zero formatting");

    /* Test unsigned formatting */
    TEST_CASE_BEGIN("Unsigned formatting");
    test_sprintf(buffer, "Unsigned: %u", 12345U);
    TEST_ASSERT_STR_EQUAL("Unsigned: 12345", buffer, "Unsigned decimal");

    /* Test hex formatting */
    TEST_CASE_BEGIN("Hexadecimal formatting");
    test_sprintf(buffer, "Hex: %x", 255);
    TEST_ASSERT_STR_EQUAL("Hex: ff", buffer, "Lowercase hex formatting");
    
    test_sprintf(buffer, "HEX: %X", 255);
    TEST_ASSERT_STR_EQUAL("HEX: FF", buffer, "Uppercase hex formatting");
    
    test_sprintf(buffer, "Addr: 0x%x", 0xDEADBEEF);
    TEST_ASSERT_STR_EQUAL("Addr: 0xdeadbeef", buffer, "Hex address formatting");

    /* Test octal formatting */
    TEST_CASE_BEGIN("Octal formatting");
    test_sprintf(buffer, "Octal: %o", 64);
    TEST_ASSERT_STR_EQUAL("Octal: 100", buffer, "Octal formatting");

    /* Test character formatting */
    TEST_CASE_BEGIN("Character formatting");
    test_sprintf(buffer, "Char: %c", 'A');
    TEST_ASSERT_STR_EQUAL("Char: A", buffer, "Character formatting");

    /* Test percent escape */
    TEST_CASE_BEGIN("Percent escape");
    test_sprintf(buffer, "100%% complete");
    TEST_ASSERT_STR_EQUAL("100% complete", buffer, "Percent sign escape");

    /* Test mixed formatting */
    TEST_CASE_BEGIN("Mixed formatting");
    test_sprintf(buffer, "%s = %d (0x%x)", "Value", 255, 255);
    TEST_ASSERT_STR_EQUAL("Value = 255 (0xff)", buffer, "Mixed format types");

    /* Test width specifiers */
    TEST_CASE_BEGIN("Width specifiers");
    test_sprintf(buffer, "%5d", 42);
    TEST_ASSERT_STR_EQUAL("   42", buffer, "Right-justified with width");
    
    test_sprintf(buffer, "%05d", 42);
    TEST_ASSERT_STR_EQUAL("00042", buffer, "Zero-padded with width");

    /* Test pointer formatting */
    TEST_CASE_BEGIN("Pointer formatting");
    test_sprintf(buffer, "Ptr: %p", (void*)0x12345678);
    /* Just verify it doesn't crash and produces something */
    TEST_ASSERT(strlen(buffer) > 0, "Pointer formatting produces output");

    TEST_SUITE_END();
}

