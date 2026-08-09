#ifndef SYSF_USEFUL_MACROS_H
#define SYSF_USEFUL_MACROS_H

#include "masks.h"
#include "overflows.h"

#define INT_DIV_CEIL(x, div) ((x) / (div) + ((x) % (div) != 0))

// Check if all the bits set in `checks` are also set in `field`.
#define BITS_ARE_SET(field, checks) (((field) & (checks)) == (checks))

/* 
 * Get the `n`th least significant byte. Eg. setting `n` to 0 will give the least significant 8 bits.
 * WARNING: Setting `n` to be larger or equal to the number of bytes in `bits` is undefined.
 * Eg. `NTH_LS_BYTE((uint8_t) <bits>, 1)` is undefined.
 */
#define NTH_LS_BYTE(bits, n) (((bits) >> ((n) * 8)) & 0xFF)

#define __NTH_MS_BYTEW(bits, n, w) ((bits >> (((w) - 8) - (n) * 8)) & 0xFF)

/*
 * Get the `n`th most significant byte. Eg. setting `n` to 0 will give the most significant 8 bits.
 * WARNING: `n` should be in the range [0, 2) otherwise the outcome is undefined.
 */
#define NTH_MS_BYTE16(bits, n) __NTH_MS_BYTEW(bits, n, 16)
/*
 * Get the `n`th most significant byte. Eg. setting `n` to 0 will give the most significant 8 bits.
 * WARNING: `n` should be in the range [0, 4) otherwise the outcome is undefined.
 */
#define NTH_MS_BYTE32(bits, n) __NTH_MS_BYTEW(bits, n, 32)
/*
 * Get the `n`th most significant byte. Eg. setting `n` to 0 will give the most significant 8 bits.
 * WARNING: `n` should be in the range [0, 8) otherwise the outcome is undefined.
 */
#define NTH_MS_BYTE64(bits, n) __NTH_MS_BYTEW(bits, n, 64)

#endif