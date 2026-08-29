#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <dbuffer.h>
#include <bounds.h>
#include "asciiargs.h"
#include "cols.h"
#include "table.h"

ARGS_STRUCT

typedef struct column_map {
    const enum ascii_arg_ind arg;
    const column *const col;
} column_map;

static const column_map COLUMN_MAP[] = {
    {.arg = CHAR_INDEX, .col = &chr_col},
    {.arg = HEX_INDEX, .col = &hex_col},
    {.arg = DECIMAL_INDEX, .col = &dec_col},
    {.arg = CARRET_INDEX, .col = &crt_col},
    {.arg = BINARY_INDEX, .col = &bin_col},
    {.arg = DESCRIPTION_INDEX, .col = &dsc_col}
};
#define COLUMN_MAP_LEN (sizeof(COLUMN_MAP) / sizeof(column_map))

ascii_col_count getColumns(column *columns) {
    ascii_col_count count = 0;
    for (size_t i = 0; i < COLUMN_MAP_LEN; i++) {
        if (boolArgSet(&ascii_args, (int) COLUMN_MAP[i].arg)) {
            columns[count++] = *COLUMN_MAP[i].col;
        }
    }
    return count;
}

uint8_t orderedSupplier(size_t index, const void *source) {
    return (void) source, (uint8_t) index;
}

uint8_t bufferSupplier(size_t index, const void *source) {
    return ((uint8_t *) source)[index];
}

int clampRange(ascii_table *table, int end, ascii_offset max) {
    ascii_count clamped_start = MIN(table->start, max);
    ascii_count clamped_end = CLAMP_BELOW_TO_CST(end, 0, max, ascii_count);
    clamped_end = MIN(clamped_end, max);
    if (clamped_start >= clamped_end) {
        printf("Expects start to come before end.\n");
        return 1;
    };
    table->cells = clamped_end - clamped_start;
    table->start = clamped_start;
    return 0;
}

int inputTable(ascii_table *table, const char *data, int end) {
    table->supplier.call = bufferSupplier;
    table->supplier.data = data;
    if (clampRange(table, end, strlen(data))) return 1;
    outputTable(table);
    return 0;
}

int pipedTable(ascii_table *table, int end) {
    dbuffer data;
    initDBuffer(&data);

    uint8_t buff[1024];
    for (size_t bytes_read = 1; bytes_read > 0; ) {
        bytes_read = fread(buff, 1, sizeof(buff), stdin);
        bufferDData(&data, buff, bytes_read);
    };

    table->supplier.call = bufferSupplier;
    table->supplier.data = data.data;
    if (clampRange(table, end, data.utilised)) return 1;
    outputTable(table);

    closeDBuffer(&data);
    return 0;
}

int defaultTable(ascii_table *table, int end) {
    table->supplier.call = orderedSupplier;
    table->supplier.data = NULL;
    if (clampRange(table, end, 127)) return 1;
    outputTable(table);
    return 0;
}

int main(int argc, char const *argv[]) {
    if (processArgs(&ascii_args, argc - 1, argv + 1)) return 1;
    int start                = getArgInt(&ascii_args, START_INDEX);
    int end                  = getArgInt(&ascii_args, END_INDEX);
    int entries_per_row      = getArgInt(&ascii_args, ENTRIES_PER_ROW_INDEX);
    const char *string_input = getArgString(&ascii_args, INPUT_INDEX);
    
    column columns[COLUMN_MAP_LEN];
    ascii_table table;
    table.columns.columns = columns;
    table.columns.column_count = getColumns(columns);
    table.hide_table = (feild) boolArgSet(&ascii_args, HIDE_TABLE_INDEX);
    table.entries_per_row = (ascii_count) CLAMP(entries_per_row, 0, 255);
    table.start = (ascii_count) MAX(start, 0);

    if (string_input != NULL) {
        return inputTable(&table, string_input, end);
    } else if (isatty(STDIN_FILENO)) {
        return defaultTable(&table, end);
    }
    return pipedTable(&table, end);
}
