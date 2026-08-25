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

#define BIT_STR_CLR "%s"
#define NIBBLE_STR_CLR BIT_STR_CLR BIT_STR_CLR BIT_STR_CLR BIT_STR_CLR
#define INT8_STR_CLR NIBBLE_STR_CLR NIBBLE_STR_CLR
#define INT16_STR_CLR INT8_STR_CLR " " INT8_STR_CLR
#define INT32_STR_CLR INT16_STR_CLR " " INT16_STR_CLR
#define INT64_STR_CLR INT32_STR_CLR " " INT32_STR_CLR

#define BIT_ARG_CLR(bit, set, unset) ((0x1 & (bit)) ? set "1\033[0m" : unset "0\033[0m")
#define NIBBLE_ARG_CLR(nibble, set, unset) BIT_ARG_CLR((nibble) >> 3, set, unset), BIT_ARG_CLR((nibble) >> 2, set, unset), BIT_ARG_CLR((nibble) >> 1, set, unset), BIT_ARG_CLR(nibble, set, unset)
#define INT8_ARG_CLR(byte, set, unset) NIBBLE_ARG_CLR((byte) >> 4, set, unset), NIBBLE_ARG_CLR(byte, set, unset)
#define INT16_ARG_CLR(int16, set, unset) INT8_ARG_CLR((int16) >> 8, set, unset), INT8_ARG_CLR(int16, set, unset)
#define INT32_ARG_CLR(int32, set, unset) INT16_ARG_CLR((int32) >> 16, set, unset), INT16_ARG_CLR(int32, set, unset)
#define INT64_ARG_CLR(int64, set, unset) INT32_ARG_CLR((int64) >> 32, set, unset), INT32_ARG_CLR(int64, set, unset)

#endif