#ifndef SYSF_BOUNDS_H
#define SYSF_BOUNDS_H

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define CLAMP_BELOW_TO(x, b, d) ((x) < (b) ? (d) : x)
#define CLAMP_BELOW_TO_CST(x, b, d, c) ((x) < (b) ? (d) : (c) (x))

#define CLAMP(a, min, max) ((a) < (min) ? (min) : ((a) > (max) ? (max) : (a)))

#endif