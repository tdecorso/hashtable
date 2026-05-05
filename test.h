#pragma once

#include <assert.h>
#include <stdio.h>

#define ANSI_BOLD  "\x1b[1m"
#define ANSI_RED   "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_CYAN  "\x1b[36m"
#define ANSI_RESET "\x1b[0m"

#define PASS_STR "[" ANSI_GREEN "PASS" ANSI_RESET "]"
#define FAIL_STR "["   ANSI_RED "FAIL" ANSI_RESET "]"

static int _failed = 0;
static int _total = 0;

#define CHECK_BOOL(b, name, ...) do {\
    _total++; \
    if ((b)) printf(PASS_STR " %s\n", name); \
    else { \
        _failed++; \
        printf(FAIL_STR " %s ", name); \
        printf(__VA_ARGS__); \
        printf("\n");\
    } \
} while (0)

#define TEST_SECTION(name) \
    printf(ANSI_BOLD "\n=== " name " ===\n\n" ANSI_RESET); \
    for (int _once = 1; _once; _once = 0) \

#define TEST_SUMMARY() do { \
    printf(ANSI_BOLD "\n\n=== SUMMARY ===\n\n" ANSI_RESET); \
    printf("Tests passed: %d\n", _total - _failed); \
    printf("Tests failed: %d\n\n", _failed); \
    if (!_failed) printf("✅ All tests passed.\n\n"); \
    else printf("❌ Some tests failed.\n\n"); \
} while (0)
