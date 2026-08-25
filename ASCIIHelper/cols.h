#ifndef SYSF_COLS_H
#define SYSF_COLS_H

#include <stddef.h>

#define DEL 127

typedef struct column {
    const char *name;
    void (*displayCell)(int chr);
    size_t len;
} column;

extern const column chr_col; 
extern const column hex_col; 
extern const column dec_col; 
extern const column bin_col; 
extern const column crt_col; 
extern const column dsc_col; 

#endif