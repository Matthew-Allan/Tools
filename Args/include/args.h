#ifndef SYSF_ARGS_H
#define SYSF_ARGS_H

#include <stdint.h>
#include <stddef.h>

enum argument_type {
    ARG_TYPE_BOOLEAN,
    ARG_TYPE_INT,
    ARG_TYPE_STRING
};

typedef struct argument {
    const char *const name;
    const char shorthand;
    const enum argument_type type;
    union {
        struct {
            uint8_t is_set : 1;
            uint8_t inv_set : 1;
        };
        int value;
        const void *ptr;
    };
} argument;

typedef int argument_ref;

typedef struct args_list {
    argument *args;
    argument_ref arg_count;
} args_list;

int processArgs(args_list *args, int argc, char const *argv[]);

int boolArgSet(const args_list *args, int arg);
const char *getArgString(const args_list *args, int arg);
int getArgInt(const args_list *args, int arg);

#endif

#ifdef DEFINE_ARGS

#include <concat.h>

#define __SUM_ARG(title, args) + 1
#define __LIST_ARG(title, args) (argument) args,
#define __ENUM_ARG(title, args) title ## _INDEX,

#define ARGS_ENUM enum CONCAT(DEFINE_ARGS, _arg_ind) { ARGS_LIST(__ENUM_ARG) };
#define ARGS_STRUCT \
static args_list CONCAT(DEFINE_ARGS, _args) = { \
    .args = (argument[]) { ARGS_LIST(__LIST_ARG) }, \
    .arg_count = ARGS_LIST(__SUM_ARG) \
};

#endif