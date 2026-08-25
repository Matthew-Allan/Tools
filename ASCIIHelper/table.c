#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include <sysmacros.h>
#include <dbuffer.h>
#include "cols.h"

typedef unsigned int uint;

#include <argshelper.h>
#define ARGS_LIST(x) \
    BOOL_ARG(x, HEX,  "hex", 'h') \
    BOOL_ARG(x, DECIMAL, "decimal", 'd') \
    BOOL_ARG(x, CARRET, "carret", 'c') \
    BOOL_ARG(x, BINARY, "binary", 'b') \
    BOOL_ARG(x, DESCRIPTION, "description", 't') \
    INV_BOOL_ARG(x, CHAR, "char", 'r') \
    INT_ARG_DEF(x, START, "start", 's', 0) \
    INT_ARG_DEF(x, END, "end", 'e', -1) \
    INT_ARG_DEF(x, ENTRIES_PER_ROW, "entries-per-row", 'p', 4) \

#define DEFINE_ARGS
#include <args.h>
INIT_ARGS(ascii);

typedef uint8_t (*data_supplier)(size_t index, void *source);

typedef struct column_map {
    const enum __ascii_arg_ind arg;
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

static size_t headerSize(const column *columns, size_t count) {
    size_t size = 1;
    for (; count; count--, columns++) {
        size += 1 + columns->len;
    }
    return size;
}

static void createHeader(const column *columns, size_t count, char *header) {
    for (; count; count--, columns++) {
        *(header++) = ' ';
        const char *name = columns->name;
        size_t i = columns->len;
        for (; i && *name != '\0'; i--) {
            *(header++) = *(name++);
        }
        for (; i; i--) {
            *(header++) = ' ';
        }
    }
}

static void printHeader(const column *columns, size_t count, size_t entries_per_row) {
    const size_t size = headerSize(columns, count);
    char header[size];
    createHeader(columns, count, header);
    header[size - 1] = '\0';
    for (size_t i = entries_per_row; i; i--) {
        printf("%s%s", header, i == 1 ? "\n" : " │");
    }
    for (size_t i = entries_per_row; i; i--) {
        for (size_t j = size; j; j--) {
            printf("─");
        }
        printf(i == 1 ? "\n" : "┼");
    }
}

static void printContents(const column *columns, data_supplier supplier, void *data, size_t count, uint rows, uint cols, uint start, uint end) {
    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            const size_t chr = c * rows + r + start;
            if (chr > end) {
                continue;
            }
            const column *column = columns;
            for (size_t j = count; j; j--, column++) {
                printf(" ");
                column->displayCell(supplier(chr, data));
            }
            if (c != cols - 1) {
                printf(" │");
            }
        }
        printf("\n");
    }
}

static void printTable(const column *columns, data_supplier supplier, void *data, size_t count, uint8_t entries_per_row, uint start, uint end) {
    if (entries_per_row == 0) {
        entries_per_row++;
    }
    const uint cells = end - start + 1;
    const uint row_count = INT_DIV_CEIL(cells, entries_per_row);
    const uint col_count = INT_DIV_CEIL(cells, row_count);

    printHeader(columns, count, col_count);
    printContents(columns, supplier, data, count, row_count, col_count, start, end);
}

uint8_t orderedSupplier(size_t index, void *source) {
    return (void) source, (uint8_t) index;
}

uint8_t bufferSupplier(size_t index, void *source) {
    return ((uint8_t *) source)[index];
}

int main(int argc, char const *argv[]) {
    if (processArgs((__args_core *) &ascii_args, argc - 1, argv + 1)) return 1;
    size_t column_count = 0;
    for (size_t i = 0; i < COLUMN_MAP_LEN; i++) {
        column_count += (size_t) boolArgSet((__args_core *) &ascii_args, (int) COLUMN_MAP[i].arg);
    }
    column columns[column_count];
    int column_index = 0;
    for (size_t i = 0; i < COLUMN_MAP_LEN; i++) {
        if (boolArgSet((__args_core *) &ascii_args, (int) COLUMN_MAP[i].arg)) {
            columns[column_index++] = *COLUMN_MAP[i].col;
        }
    }

    int start = getArgInt((__args_core *) &ascii_args, START_INDEX);
    int end = getArgInt((__args_core *) &ascii_args, END_INDEX);
    start = start < 0 ? 0 : start;

    int entries_per_row = getArgInt((__args_core *) &ascii_args, ENTRIES_PER_ROW_INDEX);
    entries_per_row = entries_per_row < 0 ? 0 : (entries_per_row > 255 ? 255 : entries_per_row);

    data_supplier supplier;
    void *source;
    if(isatty(STDIN_FILENO)) {
        supplier = orderedSupplier;
        source = NULL;
        end = end < 0 ? 127 : end;
    } else {
        dbuffer data;
        initDBuffer(&data);
        uint8_t buff[1024];
        size_t bytes_read;
        do {
            bytes_read = fread(buff, 1, sizeof(buff), stdin);
            bufferDData(&data, buff, bytes_read);
        } while(bytes_read > 0);
        end = end < 0 ? data.utilised - 1: end;
        supplier = bufferSupplier;
        source = data.data;
    }

    printTable(columns, supplier, source, column_count, (uint8_t) entries_per_row, (uint) start, (uint) end);
    if (source != NULL) {
        free(source);
    }
    return 0;
}
