#ifndef SYSF_SHA_2_H
#define SYSF_SHA_2_H

#include <stdint.h>

typedef struct SHA256_context {
    uint64_t size;
    uint32_t state[8];
    uint32_t buffer[16];
    uint16_t buffer_size;
} SHA256_context;

typedef SHA256_context SHA224_context;

typedef struct SHA512_context {
    uint64_t size[2];
    uint64_t state[8];
    uint64_t buffer[16];
    uint16_t buffer_size;
} SHA512_context;

typedef SHA512_context SHA384_context;

void SHA256_init(SHA256_context *context);
void SHA224_init(SHA224_context *context);

int SHA256_update(SHA256_context *context, const uint8_t *data, uint64_t bits);

// int SHA224_update(SHA224_context *context, const uint8_t *data, uint64_t bits);
#define SHA224_update(context, data, bits) SHA256_update(context, data, bits)

void SHA256_close(SHA256_context *context, uint8_t hash[32]);
void SHA224_close(SHA256_context *context, uint8_t hash[28]);

#endif