#ifndef SYSF_MASKS_H
#define SYSF_MASKS_H

#include <stdint.h>

#define __N_BIT_MASKW(w, bits) (((uint ## w ## _t) 1 << (bits)) - 1)
#define __N_BIT_MASKW_OFF(w, bits, off) (__N_BIT_MASKW(w, bits) << (off))
#define __N_BIT_MASKW_LFT(w, bits)  __N_BIT_MASKW_OFF(w, bits, w - (bits))

// WARNING: `bits` must be in the range [0, 8). This means that it should NOT be 8.
#define N_BIT_MASK8(bits) __N_BIT_MASKW(8, bits)
// WARNING: `bits` AND `off` must be in the range [0, 8). This means that they should NOT be 8.
#define N_BIT_MASK8_OFF(bits, off) __N_BIT_MASKW_OFF(8, bits, off)
// WARNING `bits` must be in the range (0, 8). This means that it should NOT be 8 OR 0.
#define N_BIT_MASK8_LFT(bits) __N_BIT_MASKW_LFT(8, bits)

// WARNING: `bits` must be in the range [0, 16). This means that it should NOT be 16.
#define N_BIT_MASK16(bits) __N_BIT_MASKW(16, bits)
// WARNING: `bits` AND `off` must be in the range [0, 16). This means that they should NOT be 16.
#define N_BIT_MASK16_OFF(bits, off) __N_BIT_MASKW_OFF(16, bits, off)
// WARNING `bits` must be in the range (0, 16). This means that it should NOT be 16 OR 0.
#define N_BIT_MASK16_LFT(bits) __N_BIT_MASKW_LFT(16, bits)

// WARNING: `bits` must be in the range [0, 32). This means that it should NOT be 32.
#define N_BIT_MASK32(bits) __N_BIT_MASKW(32, bits)
// WARNING: `bits` AND `off` must be in the range [0, 32). This means that they should NOT be 32.
#define N_BIT_MASK32_OFF(bits, off) __N_BIT_MASKW_OFF(32, bits, off)
// WARNING `bits` must be in the range (0, 32). This means that it should NOT be 32 OR 0.
#define N_BIT_MASK32_LFT(bits) __N_BIT_MASKW_LFT(32, bits)

// WARNING: `bits` must be in the range [0, 64). This means that it should NOT be 64.
#define N_BIT_MASK64(bits) __N_BIT_MASKW(64, bits)
// WARNING: `bits` AND `off` must be in the range [0, 64). This means that they should NOT be 64.
#define N_BIT_MASK64_OFF(bits, off) __N_BIT_MASKW_OFF(64, bits, off)
// WARNING `bits` must be in the range (0, 64). This means that it should NOT be 64 OR 0.
#define N_BIT_MASK64_LFT(bits) __N_BIT_MASKW_LFT(64, bits)

#endif