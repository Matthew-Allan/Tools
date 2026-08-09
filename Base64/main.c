#include <base64.h>

#include <stdio.h>

int decodeString(const char *encoded, size_t size, uint8_t flags) {
    printf("%.*s\n", (int) size, encoded);
    size_t decode_size;
    if (base64_decodedSize(encoded, size, flags, &decode_size) == -1) {
        printf("Failed to get decode size\n");
        return 1;
    }
    char decoded[decode_size];
    size_t bytes_produced;
    if (base64_decode(encoded, size, (uint8_t *) decoded, flags, &bytes_produced) == -1) {
        printf("Failed to decode\n");
        return 1;
    }
    printf("%.*s\n", (int) bytes_produced, decoded);
    return 0;
}

int translateString(const char *str, size_t size, uint8_t flags) {
    size_t encode_size;
    if (base64_encodedSize(size, flags, &encode_size) == -1) {
        printf("Failed to get encode size\n");
        return 1;
    }
    char output[encode_size + 1];
    output[encode_size] = '\0';
    encode_size = base64_encode((const uint8_t *) str, size, output, flags);
    return decodeString(output, encode_size, flags);
}

int main() {
    const char message[] = "Hello Megan my love. Do you think I'm a nerd?";
    translateString(message, sizeof(message), B64_NO_PADDING);
    return 0;
}
