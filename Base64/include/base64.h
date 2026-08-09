#ifndef SYSF_TOOLS_BASE_64_H
#define SYSF_TOOLS_BASE_64_H

#include <stdint.h>
#include <stddef.h>

#define B64_NO_PADDING 1 /* Do not add padding to the end of the output */
#define B64_URL_SAFE   2 /* Use '-' and '_' characters instead of '+' and '/' */

/*
 * Populate `dst` with the Base64 representation of the provided `src` bytes.
 * Expects `dst` to be large enough to fit the representation in without a 
 * null terminator. `base64_encodedSize()` can be used to find the correct size.
 * Returns the size of the encoded data (should be the same as the output of
 * `base64_encodedSize()`)
 */
size_t base64_encode(const uint8_t *src, size_t size, char *dst, uint8_t flags);

/*
 * Populate `dst` with the bytes decoded from the provided `src` Base64.
 * Expects `dst` to be large enough to fit the decoded bytes in without a 
 * null terminator (unless one is encoded). `base64_decodedSize()` can
 * be used to estimate the correct size.
 * Returns 0 if succeeded or -1 if there was an error during encoding.
 * bytes_produced should have the same value as the output of
 * `base64_decodedSize()`. Passing in `NULL` is allowed too if the decoded
 * len is not required.
 */
int base64_decode(const char *src, size_t size, uint8_t *dst, uint8_t flags, size_t *bytes_produced);

/*
 * Get the number of bytes required to store the output of encoding a
 * Base64 string.
 */
int base64_encodedSize(size_t size, uint8_t flags, size_t *out);

/*
 * Get the number of bytes required to store the output of decoding a
 * Base64 string.
 */
int base64_decodedSize(const char *src, size_t size, uint8_t flags, size_t *out);

// size_t base64_decodeURL(const uint8_t *src, size_t size, char *dst)
#define base64_decodeURL(src, size, dst) base64_encode(src, size, dst, B64_URL_SAFE | B64_NO_PADDING)

// size_t base64_decodeURL(const uint8_t *src, size_t size, char *dst)
#define base64_decodeSTD(src, size, dst) base64_encode(src, size, dst, 0)

// int base64_decodedSize(size_t size, size_t *out);
#define base64_decodedSizeNoPad(size, out) base64_decodedSize(NULL, size, B64_NO_PADDING, out)

// int base64_decodedSize(const char *src, size_t size, size_t *out);
#define base64_decodedSizeSTD(src, size, out) base64_decodedSize(src, size, 0, out)

#endif