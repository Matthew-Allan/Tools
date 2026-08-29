#include "table.h"

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sysmacros.h>
#include <dbuffer.h>
#include "cols.h"
#include "asciiargs.h"

static size_t headerSize(const column_list *columns) {
    size_t size = 1;
    const column *column = columns->columns;
    for (ascii_count count = columns->column_count; count; count--, column++) {
        size += 1 + column->len;
    }
    return size;
}

static void createHeader(const column_list *columns, char *header) {
    const column *column = columns->columns;
    for (ascii_count count = columns->column_count; count; count--, column++) {
        *(header++) = ' ';
        const char *name = column->name;
        size_t i = column->len;
        for (; i && *name != '\0'; i--) {
            *(header++) = *(name++);
        }
        for (; i; i--) {
            *(header++) = ' ';
        }
    }
}

static void printHeader(const column_list *columns, size_t entries_per_row) {
    const size_t size = headerSize(columns);
    char header[size];
    createHeader(columns, header);
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

static void printContents(const ascii_table *table, ascii_count rows, ascii_count cols) {
    for (ascii_count r = 0; r < rows; r++) {
        for (ascii_count c = 0; c < cols; c++) {
            const size_t chr = c * rows + r;
            if (chr >= table->cells) {
                continue;
            }
            const column *column = table->columns.columns;
            for (size_t j = table->columns.column_count; j; j--, column++) {
                printf(" ");
                column->displayCell(table->supplier.call(chr + table->start, table->supplier.data));
            }
            if (c != cols - 1) {
                if (!table->hide_table) printf(" │");
            }
        }
        printf("\n");
    }
}

void outputTable(const ascii_table *table) {
    const ascii_count entries_per_row = table->entries_per_row + !table->entries_per_row;
    const ascii_count row_count = INT_DIV_CEIL(table->cells, entries_per_row);
    const ascii_count col_count = INT_DIV_CEIL(table->cells, row_count);
    if (!table->hide_table) printHeader(&table->columns, col_count);
    printContents(table, row_count, col_count);
}