#include <base64.h>
#include <sysmacros.h>

#include <limits.h>

// Padding representation in the decode table
#define PD 64

// A table containing the standard Base64 encodings, specifying
// the array length to remove the null terminator. 
static const char _encode_table[64] = {
    "ABCDEFGHIJKLMNOP"
    "QRSTUVWXYZabcdef"
    "ghijklmnopqrstuv"
    "wxyz0123456789+/"
};

// A table containing the inverse of the standard Base64 encodings.
static const int8_t _decode_table[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, PD, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};

static char encodeBits(uint8_t bits, uint8_t flags) {
    if (BITS_ARE_SET(flags, B64_URL_SAFE)) {
        switch (bits) {
            case 62: return '-';
            case 63: return '_';
        }
    }
    return _encode_table[bits];
}

size_t base64_encode(const uint8_t *src, size_t size, char *dst, uint8_t flags) {
    // Set up a bit buffer with size keeping track of how many bits are in use.
    uint16_t bit_buffer = 0;
    uint8_t bit_buffer_size = 0;

    // Keep track of how large the output size is.
    size_t encoded_size = 0;

    // Work out how much padding should be used off the bat.
    uint8_t padding = BITS_ARE_SET(flags, B64_NO_PADDING) ? 0 : (3 - (size % 3)) % 3;
    for (; size > 0; size--, src++) {
        // Add src byte to buffer: ____________bbbb -> ____bbbbssssssss
        bit_buffer = (bit_buffer << 8) | *src;
        bit_buffer_size += 8;

        // While there are enough bits, consume a 6-bit symbol.
        for (; bit_buffer_size >= 6; bit_buffer &= N_BIT_MASK16(bit_buffer_size), encoded_size++) {
            bit_buffer_size -= 6;
            *(dst++) = encodeBits(bit_buffer >> bit_buffer_size, flags);
        }
    }
    // If there are still bits left in the buffer, pad with 0s to make 6 bits and consume.
    if (bit_buffer_size > 0) {
        encoded_size++;
        *(dst++) = encodeBits(bit_buffer << (6 - bit_buffer_size), flags);
    }
    // Add required padding symbols up to a multiple of 4 symbols.
    for (; padding > 0; padding--, encoded_size++) {
        *(dst++) = '=';
    }
    return encoded_size;
}

static int8_t decodeChar(unsigned char chr, uint8_t flags) {
    // Bounds check
    if (chr > 127) {
        return -1;
    }

    // Swap out '+' and '/' for '-' and '_' in URL mode
    if (BITS_ARE_SET(flags, B64_URL_SAFE)) {
        switch (chr) {
            case '-': return 62;
            case '_': return 63;
            case '+': case '/': return -1;
        }
    }

    // Get the 6 bit value from the table and return, checking if padding symbols are allowed.
    int8_t bits = _decode_table[chr];
    return bits == PD && BITS_ARE_SET(flags, B64_NO_PADDING) ? -1 : bits;
}

int base64_decode(const char *src, size_t size, uint8_t *dst, uint8_t flags, size_t *bytes_produced) {
    // Allow for bytes_produced to not be requested.
    size_t fb_produced;
    if (bytes_produced == NULL) bytes_produced = &fb_produced;
    *bytes_produced = 0;

    // Return if the length of the encoded string is 0.
    if (size == 0) return 0;

    // Check if the size is valid for padded encodings.
    uint8_t has_padding = !BITS_ARE_SET(flags, B64_NO_PADDING);
    if (has_padding && size % 4 != 0) return -1;
    
    // Set up a bit buffer with size keeping track of how many bits are in use.
    uint16_t bit_buffer = 0;
    int8_t bit_buffer_size = 0;
    for (; size > 0; size--, src++) {
        // Get the 6 bit value from a Base64 symbol.
        int8_t value = decodeChar((unsigned char) *src, flags);

        // Check if the string encounters a padding or invalid symbol.
        if (value == PD) break;
        if (value == -1) return -1;

        // Add src byte to buffer: ____________bbbb -> ______bbbbssssss
        bit_buffer = (bit_buffer << 6) | value;
        bit_buffer_size += 6;

        // While there are enough bits, consume an 8-bit symbol.
        for (; bit_buffer_size >= 8; bit_buffer &= N_BIT_MASK16(bit_buffer_size), (*bytes_produced)++) {
            bit_buffer_size -= 8;
            *(dst++) = (uint8_t) (bit_buffer >> bit_buffer_size);
        }
    }

    // Ensure there are no remaining set bits in the buffer. 
    // We assume that the remaining 0s are padding bits.
    if (bit_buffer != 0) return -1;
    if (has_padding) {
        // Ensure that there is the right amount of padding symbols left over.
        if (size > 2) return -1;
        for (; size > 0; size--, src++) {
            if (*src != '=') return -1;
        }
        return 0;
    } else {
        // Return -1 if the number of bits left in the buffer matches the expected 0-padding len.
        return -(bit_buffer_size != (int[]){0, 4, 2}[*bytes_produced % 3]);
    }

}

int base64_encodedSize(size_t size, uint8_t flags, size_t *out) {
    *out = 0;
    if (size == 0) return 0;
    size_t tris = size / 3;
    size_t remainder = size % 3;
    
    size_t encoded_length;
    if (BITS_ARE_SET(flags, B64_NO_PADDING)) {
        size_t remainder_chars = INT_DIV_CEIL(remainder * 4, 3);
        if (MULT_ADD_OVERFLOWS(tris, 4, remainder_chars)) return -1;
        encoded_length = tris * 4 + remainder_chars;
    } else {
        tris += remainder != 0;
        if (MULT_OVERFLOWS(tris, 4)) return -1;
        encoded_length = tris * 4;
    }

    *out = encoded_length;
    return 0;
}

int base64_decodedSize(const char *src, size_t size, uint8_t flags, size_t *out) {
    *out = 0;
    if (size == 0) return 0;

    size_t len;
    if (BITS_ARE_SET(flags, B64_NO_PADDING)) {
        len = (size / 4) * 3 + (3 * (size % 4)) / 4; // (size * 3) / 4 without overflow.
    } else {
        if (size % 4 != 0 || src[size - 3] == '=' || src[size - 4] == '=') return -1;
        int padding = src[size - 1] == '=';
        padding += padding && src[size - 2] == '=';
        len = ((size / 4) * 3) - (size_t) padding;
    }
    *out = len;
    return 0;
}