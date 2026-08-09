#ifndef SYSF_OVERFLOWS_H
#define SYSF_OVERFLOWS_H

#include <sys/types.h>

// Check if the result of `x * m` overflows a `size_t`.
#define MULT_OVERFLOWS(x, m) ((x) > SIZE_MAX / (m))

// Check if the result of `x + m` overflows a `size_t`.
#define ADD_OVERFLOWS(x, a) ((x) > SIZE_MAX - (a))

// Check if the result of `x * m + a` overflows a `size_t`.
#define MULT_ADD_OVERFLOWS(x, m, a) ((x) > (SIZE_MAX - (a)) / (m))

// Check if the result of `(x + a) * m` overflows a `size_t`.
#define ADD_MULT_OVERFLOWS(x, a, m) ((a) > SIZE_MAX / (m) || (x) > SIZE_MAX / (m) - (a))

#endif