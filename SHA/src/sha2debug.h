#ifndef SHA_2_DEBUG_H
#define SHA_2_DEBUG_H

#include <stdint.h>
#include <stddef.h>

void print64BitInts(uint64_t *words, size_t size);
void print32BitInts(uint32_t *words, size_t size);
void print16BitInts(uint16_t *words, size_t size);
void print8BitInts(uint8_t *bytes, size_t size);

#endif