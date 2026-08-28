#include <args.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

static int parseInt(argument *arg, char const *next) {
    if (next == NULL) {
        printf("Expected an int after option \"--%s\" but found nothing.\n", arg->name);
        return 1;
    }
    char *endpnt;
    errno = 0;
    long value = strtol(next, &endpnt, 10);
    if (endpnt == next || *endpnt != '\0' || value > INT_MAX || value < INT_MIN || errno == ERANGE) {
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

static int typeSwitch(args_list *args, argument_ref arg, char const *next, int *incr) {
    switch (args->args[arg].type) {
        case ARG_TYPE_BOOLEAN: args->args[arg].is_set = 1; return 0;
        case ARG_TYPE_INT: (*incr)++; return parseInt(args->args + arg, next);
        case ARG_TYPE_STRING: (*incr)++; return parseString(args->args + arg, next);
    }
    return 1;
}

int processShorthand(args_list *args, char const *shorthands, char const *next, int *incr) {
    if (*shorthands == '\0') {
        printf("Unknown option '-'\n");
        return 1;
    }
    for (; *shorthands != '\0'; shorthands++) {
        argument_ref i = 0;
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

int processArg(args_list *args, char const *arg, char const *next, int *incr) {
    argument_ref i = 0;
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

int processArgs(args_list *args, int argc, char const *argv[]) {
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

static const argument *getArg(const args_list *args, argument_ref arg) {
    return (arg < 0 || arg >= args->arg_count) ? NULL : args->args + arg;
}

static int isType(const argument *arg, enum argument_type type) {
    return arg != NULL && arg->type == type;
}

int boolArgSet(const args_list *args, argument_ref arg) {
    const argument *argmnt = getArg(args, arg);
    return isType(argmnt, ARG_TYPE_BOOLEAN) && (argmnt->is_set ^ argmnt->inv_set);
}

const char *getArgString(const args_list *args, argument_ref arg) {
    const argument *argmnt = getArg(args, arg);
    return (const char *) (isType(argmnt, ARG_TYPE_STRING) ? argmnt->ptr : NULL);
}

int getArgInt(const args_list *args, argument_ref arg) {
    const argument *argmnt = getArg(args, arg);
    return isType(argmnt, ARG_TYPE_INT) ? argmnt->value : 0;
}