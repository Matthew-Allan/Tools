#include <args.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

static int parseInt(argument *arg, char const *next) {
    if (next == NULL) {
        printf("Expected an int after option \"--%s\" but found nothing.\n", arg->name);
        return 1;
    }
    char *endpnt;
    long value = strtol(next, &endpnt, 10);
    if (*endpnt != '\0' || value > INT_MAX || value < INT_MIN) {
        printf("Expected an int after option \"--%s\" but found \"%s\".\n", arg->name, next);
        return 1;
    }
    arg->value = (int) value;
    return 0;
}

static int parseString(argument *arg, char const *next) {
    if (next == NULL) {
        printf("Expected a string after option \"--%s\" but found nothing.\n", arg->name);
        return 1;
    }
    arg->ptr = next;
    return 0;
}

static int typeSwitch(__args_core *args, int arg, char const *next, int *incr) {
    switch (args->args[arg].type) {
        case ARG_TYPE_BOOLEAN: args->args[arg].is_set = 1; return 0;
        case ARG_TYPE_INT: (*incr)++; return parseInt(args->args + arg, next);
        case ARG_TYPE_STRING: (*incr)++; return parseString(args->args + arg, next);
    }
    return 1;
}

int processShorthand(__args_core *args, char const *shorthands, char const *next, int *incr) {
    for (; *shorthands != '\0'; shorthands++) {
        size_t i = 0;
        for (; i < args->arg_count; i++) {
            if (args->args[i].shorthand == *shorthands) {
                if (typeSwitch(args, i, *(shorthands + 1) == '\0' ? next : NULL, incr)) return 1;
                break;
            }
        }
        if (i == args->arg_count) {
            printf("Unknown option '%c'\n", *shorthands);
            return 1;
        }
    }
    return 0;
}

int processArg(__args_core *args, char const *arg, char const *next, int *incr) {
    size_t i = 0;
    for (; i < args->arg_count; i++) {
        if (!strcmp(args->args[i].name, arg)) {
            if (typeSwitch(args, i, next, incr)) return 1;
            break;
        }
    }
    if (i == args->arg_count) {
        printf("Unknown option \"%s\"\n", arg);
        return 1;
    }
    return 0;
}

int processArgs(__args_core *args, int argc, char const *argv[]) {
    (void) args;
    for (int i = 0; i < argc; i++) {
        char const *arg = argv[i];
        if (*(arg++) != '-') {
            printf("Unexpected arg: \"%s\"\n", argv[i]);
            return 1;
        }
        char const *next = i == argc - 1 ? NULL : argv[i + 1];
        if (*arg != '-') {
            if (processShorthand(args, arg, next, &i)) return 1;
        } else {
            if (processArg(args, arg + 1, next, &i)) return 1;
        } 
    }
    return 0;
}

int boolArgSet(const __args_core *args, int arg) {
    const argument *argmnt = &args->args[arg];
    return argmnt->is_set ^ argmnt->inv_set;
}

const char *getArgString(const __args_core *args, int arg) {
    const argument *argmnt = &args->args[arg];
    return (const char *) (argmnt->type != ARG_TYPE_STRING ? NULL : argmnt->ptr);
}

int getArgInt(const __args_core *args, int arg) {
    const argument *argmnt = &args->args[arg];
    return argmnt->type != ARG_TYPE_INT ? 0 : argmnt->value;
}