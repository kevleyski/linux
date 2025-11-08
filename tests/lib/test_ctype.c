/*
 * Unit tests for ctype functions
 * Tests character classification and conversion
 */

#include <stdio.h>
#include "../test_framework.h"

/* Undefine any conflicting macros before including kernel headers */
#undef NULL
#undef EOF

/* Include the kernel's ctype definitions */
#include <linux/ctype.h>

int main(void) {
    TEST_SUITE_BEGIN("CType Functions");

    /* Test isdigit */
    TEST_CASE_BEGIN("isdigit");
    TEST_ASSERT(isdigit('0'), "'0' is a digit");
    TEST_ASSERT(isdigit('5'), "'5' is a digit");
    TEST_ASSERT(isdigit('9'), "'9' is a digit");
    TEST_ASSERT(!isdigit('a'), "'a' is not a digit");
    TEST_ASSERT(!isdigit('A'), "'A' is not a digit");
    TEST_ASSERT(!isdigit(' '), "space is not a digit");

    /* Test isalpha */
    TEST_CASE_BEGIN("isalpha");
    TEST_ASSERT(isalpha('a'), "'a' is alpha");
    TEST_ASSERT(isalpha('z'), "'z' is alpha");
    TEST_ASSERT(isalpha('A'), "'A' is alpha");
    TEST_ASSERT(isalpha('Z'), "'Z' is alpha");
    TEST_ASSERT(!isalpha('0'), "'0' is not alpha");
    TEST_ASSERT(!isalpha('9'), "'9' is not alpha");
    TEST_ASSERT(!isalpha(' '), "space is not alpha");

    /* Test isalnum */
    TEST_CASE_BEGIN("isalnum");
    TEST_ASSERT(isalnum('a'), "'a' is alnum");
    TEST_ASSERT(isalnum('Z'), "'Z' is alnum");
    TEST_ASSERT(isalnum('0'), "'0' is alnum");
    TEST_ASSERT(isalnum('9'), "'9' is alnum");
    TEST_ASSERT(!isalnum(' '), "space is not alnum");
    TEST_ASSERT(!isalnum('!'), "'!' is not alnum");

    /* Test isupper */
    TEST_CASE_BEGIN("isupper");
    TEST_ASSERT(isupper('A'), "'A' is upper");
    TEST_ASSERT(isupper('Z'), "'Z' is upper");
    TEST_ASSERT(!isupper('a'), "'a' is not upper");
    TEST_ASSERT(!isupper('z'), "'z' is not upper");
    TEST_ASSERT(!isupper('0'), "'0' is not upper");

    /* Test islower */
    TEST_CASE_BEGIN("islower");
    TEST_ASSERT(islower('a'), "'a' is lower");
    TEST_ASSERT(islower('z'), "'z' is lower");
    TEST_ASSERT(!islower('A'), "'A' is not lower");
    TEST_ASSERT(!islower('Z'), "'Z' is not lower");
    TEST_ASSERT(!islower('0'), "'0' is not lower");

    /* Test isspace */
    TEST_CASE_BEGIN("isspace");
    TEST_ASSERT(isspace(' '), "space is space");
    TEST_ASSERT(isspace('\t'), "tab is space");
    TEST_ASSERT(isspace('\n'), "newline is space");
    TEST_ASSERT(isspace('\r'), "carriage return is space");
    TEST_ASSERT(!isspace('a'), "'a' is not space");
    TEST_ASSERT(!isspace('0'), "'0' is not space");

    /* Test ispunct */
    TEST_CASE_BEGIN("ispunct");
    TEST_ASSERT(ispunct('!'), "'!' is punct");
    TEST_ASSERT(ispunct('.'), "'.' is punct");
    TEST_ASSERT(ispunct(','), "',' is punct");
    TEST_ASSERT(ispunct(';'), "';' is punct");
    TEST_ASSERT(!ispunct('a'), "'a' is not punct");
    TEST_ASSERT(!ispunct('0'), "'0' is not punct");
    TEST_ASSERT(!ispunct(' '), "space is not punct");

    /* Test isxdigit */
    TEST_CASE_BEGIN("isxdigit");
    TEST_ASSERT(isxdigit('0'), "'0' is hex digit");
    TEST_ASSERT(isxdigit('9'), "'9' is hex digit");
    TEST_ASSERT(isxdigit('a'), "'a' is hex digit");
    TEST_ASSERT(isxdigit('f'), "'f' is hex digit");
    TEST_ASSERT(isxdigit('A'), "'A' is hex digit");
    TEST_ASSERT(isxdigit('F'), "'F' is hex digit");
    TEST_ASSERT(!isxdigit('g'), "'g' is not hex digit");
    TEST_ASSERT(!isxdigit('G'), "'G' is not hex digit");

    /* Test iscntrl */
    TEST_CASE_BEGIN("iscntrl");
    TEST_ASSERT(iscntrl('\0'), "null is control");
    TEST_ASSERT(iscntrl('\t'), "tab is control");
    TEST_ASSERT(iscntrl('\n'), "newline is control");
    TEST_ASSERT(!iscntrl('a'), "'a' is not control");
    TEST_ASSERT(!iscntrl(' '), "space is not control (it's printable)");

    /* Test isprint */
    TEST_CASE_BEGIN("isprint");
    TEST_ASSERT(isprint(' '), "space is printable");
    TEST_ASSERT(isprint('a'), "'a' is printable");
    TEST_ASSERT(isprint('Z'), "'Z' is printable");
    TEST_ASSERT(isprint('0'), "'0' is printable");
    TEST_ASSERT(isprint('!'), "'!' is printable");
    TEST_ASSERT(!isprint('\t'), "tab is not printable");
    TEST_ASSERT(!isprint('\n'), "newline is not printable");

    /* Test toupper */
    TEST_CASE_BEGIN("toupper");
    TEST_ASSERT_EQUAL('A', toupper('a'), "toupper('a') == 'A'");
    TEST_ASSERT_EQUAL('Z', toupper('z'), "toupper('z') == 'Z'");
    TEST_ASSERT_EQUAL('A', toupper('A'), "toupper('A') == 'A' (unchanged)");
    TEST_ASSERT_EQUAL('0', toupper('0'), "toupper('0') == '0' (unchanged)");

    /* Test tolower */
    TEST_CASE_BEGIN("tolower");
    TEST_ASSERT_EQUAL('a', tolower('A'), "tolower('A') == 'a'");
    TEST_ASSERT_EQUAL('z', tolower('Z'), "tolower('Z') == 'z'");
    TEST_ASSERT_EQUAL('a', tolower('a'), "tolower('a') == 'a' (unchanged)");
    TEST_ASSERT_EQUAL('0', tolower('0'), "tolower('0') == '0' (unchanged)");

    TEST_SUITE_END();
}

