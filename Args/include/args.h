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

typedef struct __args_core {
    argument *args;
    size_t arg_count;
} __args_core;

int processArgs(__args_core *args, int argc, char const *argv[]);

int boolArgSet(const __args_core *args, int arg);
const char *getArgString(const __args_core *args, int arg);
int getArgInt(const __args_core *args, int arg);

#endif

#ifdef DEFINE_ARGS

#define __SUM_ARG(_title, _args) + 1
#define __LIST_ARG(_title, _args) (argument) _args,
#define __ENUM_ARG(_title, _args) _title ## _INDEX,

#define INIT_ARGS(name) \
typedef struct __ ## name ## _args { \
    __args_core _args_core; \
} __ ## name ## _args; \
 \
enum __ ## name ## _arg_ind { \
    ARGS_LIST(__ENUM_ARG) \
}; \
static __ ## name ## _args name ## _args = { \
    ._args_core = { \
        .args = (argument[]) { \
            ARGS_LIST(__LIST_ARG) \
        }, \
        .arg_count = ARGS_LIST(__SUM_ARG) \
    } \
};

#endif