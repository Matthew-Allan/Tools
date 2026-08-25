#ifndef SYSF_DYNAMIC_LIST_H
#define SYSF_DYNAMIC_LIST_H

#include <stddef.h>

#ifndef DYNAMIC_BUFFER_INITIAL_SIZE
#define DYNAMIC_BUFFER_INITIAL_SIZE 16
#endif

#ifndef DYNAMIC_LIST_SCALING_FACTOR
#define DYNAMIC_LIST_SCALING_FACTOR 1.5
#endif

typedef struct dbuffer {
    void *data;
    size_t assigned;
    size_t utilised;
} dbuffer;

int bufferDData(dbuffer *buff, void *src, size_t size);
int getDData(dbuffer *buff, void *dst, size_t offset, size_t size);

int initDBuffer(dbuffer *buff);
void closeDBuffer(dbuffer *buff);
void emptyDBuffer(dbuffer *buff);

#endif