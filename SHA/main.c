#include <sha2.h>

#include <stdio.h>

int hash_data256(const uint8_t *data, uint64_t size, uint8_t hash[32]) {
    SHA256_context context;
    SHA256_init(&context);
    if (SHA256_update(&context, data, size) == -1) {
        return -1;
    }
    SHA256_close(&context, hash);
    printf("hash: ");
    for (int i = 0; i < 32; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    return 0;
}

int hash_data224(const uint8_t *data, uint64_t size, uint8_t hash[24]) {
    SHA224_context context;
    SHA224_init(&context);
    if (SHA224_update(&context, data, size) == -1) {
        return -1;
    }
    SHA224_close(&context, hash);
    printf("hash: ");
    for (int i = 0; i < 28; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
    return 0;
}

int main() {
    uint8_t hash[32] = {0};
    hash_data224((uint8_t *) "Hello, world!", 13 * 8, (uint8_t *) hash);
    return 0;
}