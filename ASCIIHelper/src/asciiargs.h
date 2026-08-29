#ifndef SYSF_ASCII_ARGS_H
#define SYSF_ASCII_ARGS_H

#include <argshelper.h>
#define ARGS_LIST(x) \
    BOOL_ARG(x, HEX,  "hex", 'h') \
    BOOL_ARG(x, DECIMAL, "decimal", 'd') \
    BOOL_ARG(x, CARRET, "carret", 'c') \
    BOOL_ARG(x, BINARY, "binary", 'b') \
    BOOL_ARG(x, DESCRIPTION, "description", 't') \
    BOOL_ARG(x, HIDE_TABLE, "hide-table", 'x') \
    INV_BOOL_ARG(x, CHAR, "char", 'r') \
    STRING_ARG(x, INPUT, "input", 'i') \
    INT_ARG_DEF(x, START, "start", 's', 0) \
    INT_ARG_DEF(x, END, "end", 'e', -1) \
    INT_ARG_DEF(x, ENTRIES_PER_ROW, "entries-per-row", 'p', 4)

#define DEFINE_ARGS ascii
#include <args.h>
ARGS_ENUM

#endif