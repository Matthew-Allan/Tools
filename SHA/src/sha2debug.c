#include "sha2debug.h"

#include <stdio.h>
#include <bitprint.h>

#define printWBitInts(words, size, w) { \
    uint8_t words_per_row = 64 / w; \
    printf(size > words_per_row ? "msb: " : "bts: "); \
    for (uint8_t max = words_per_row; size > 0 && max > 0; size--, max--, words++) printf(INT ## w ## _STR " ", INT ## w ## _ARG(*words)); \
    printf("\n"); \
    if (size == 0) return; \
    while (size > words_per_row) { \
        printf("     "); \
        for (int i = 0; i < words_per_row; i++, words++, size--) printf(INT ## w ## _STR " ", INT ## w ## _ARG(*words)); \
        printf("\n"); \
    } \
    printf("lsb: "); \
    for (; size > 0; size--, words++) printf(INT ## w ## _STR "%c", INT ## w ## _ARG(*words), size == 1 ? '\n' : ' '); \
}

void print64BitInts(uint64_t *words, size_t size) {
    printWBitInts(words, size, 64)
}

void print32BitInts(uint32_t *words, size_t size) {
    printWBitInts(words, size, 32)
}

void print16BitInts(uint16_t *words, size_t size) {
    printWBitInts(words, size, 16)
}

void print8BitInts(uint8_t *words, size_t size) {
    printWBitInts(words, size, 8)
}