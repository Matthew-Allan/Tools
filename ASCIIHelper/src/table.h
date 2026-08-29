#ifndef SYSF_ASCII_TABLE_H
#define SYSF_ASCII_TABLE_H

#include <stddef.h>
#include <stdint.h>

#include "cols.h"

typedef unsigned int ascii_offset;
typedef unsigned int ascii_count;
typedef unsigned int ascii_col_count;
typedef unsigned int feild;

typedef struct column_list {
    column *columns;
    ascii_col_count column_count;
} column_list;

typedef struct data_supplier {
    uint8_t (*call)(size_t index, const void *source);
    const void *data;
} data_supplier;

typedef struct ascii_table {
    data_supplier supplier;
    column_list columns;
    ascii_offset start;
    ascii_count cells;
    ascii_count entries_per_row;
    feild hide_table : 1;
} ascii_table;

void outputTable(const ascii_table *table);

#endif