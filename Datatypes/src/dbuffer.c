#include <dbuffer.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <overflows.h>

static int setSize(dbuffer *buff, size_t size) {
    void * const data = malloc(size);
    if (data == NULL) return 1;
    buff->data = data;
    buff->assigned = size;
    return 0;
}

static int resize(dbuffer *buff, size_t min) {
    size_t new_assigned = buff->assigned;
    if (DYNAMIC_LIST_SCALING_FACTOR < 1) return 1;
    while (new_assigned < min) {
        const double scaled = new_assigned * DYNAMIC_LIST_SCALING_FACTOR;
        size_t candidate;
        if (scaled >= SIZE_MAX - 65536) {
            candidate = min;
        } else {
            candidate = (size_t) scaled;
        }
        new_assigned = candidate == new_assigned ? candidate + 1 : candidate;
    }
    void * const old = buff->data;
    if(setSize(buff, new_assigned)) return 1;
    memcpy(buff->data, old, buff->utilised);
    free(old);
    return 0;
}

int bufferDData(dbuffer *buff, void *src, size_t size) {
    if (size == 0) return 0;
    if (ADD_OVERFLOWS(buff->utilised, size)) return 1;
    const size_t new_utilised = buff->utilised + size;
    if (new_utilised > buff->assigned) {
        if (resize(buff, new_utilised)) return 1;
    }
    memcpy((uint8_t *) buff->data + buff->utilised, src, size);
    buff->utilised = new_utilised;
    return 0;
}

int getDData(dbuffer *buff, void *dst, size_t offset, size_t size) {
    if (ADD_OVERFLOWS(offset, size) || offset + size > buff->utilised) return 1;
    memcpy(dst, (uint8_t *) buff->data + offset, size);
    return 0;
}

int initDBuffer(dbuffer *buff) {
    if (setSize(buff, DYNAMIC_BUFFER_INITIAL_SIZE)) return 1;
    buff->utilised = 0;
    return 0;
}

void closeDBuffer(dbuffer *buff) {
    free(buff->data);
    buff->data = NULL;
    buff->assigned = 0;
    buff->utilised = 0;
}

void emptyDBuffer(dbuffer *buff) {
    buff->utilised = 0;
}