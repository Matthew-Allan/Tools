#ifndef SYSF_BIT_PRINT_H
#define SYSF_BIT_PRINT_H

#define BIT_STR "%c"
#define NIBBLE_STR BIT_STR BIT_STR BIT_STR BIT_STR
#define INT8_STR NIBBLE_STR NIBBLE_STR
#define INT16_STR INT8_STR " " INT8_STR
#define INT32_STR INT16_STR " " INT16_STR
#define INT64_STR INT32_STR " " INT32_STR

#define BIT_ARG(bit) ((char) ((0x1 & (bit)) + '0'))
#define NIBBLE_ARG(nibble) BIT_ARG((nibble) >> 3), BIT_ARG((nibble) >> 2), BIT_ARG((nibble) >> 1), BIT_ARG(nibble)
#define INT8_ARG(byte) NIBBLE_ARG((byte) >> 4), NIBBLE_ARG(byte)
#define INT16_ARG(int16) INT8_ARG((int16) >> 8), INT8_ARG(int16)
#define INT32_ARG(int32) INT16_ARG((int32) >> 16), INT16_ARG(int32)
#define INT64_ARG(int64) INT32_ARG((int64) >> 32), INT32_ARG(int64)

#endif