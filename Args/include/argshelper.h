#ifndef SYSF_ARGS_HELPER_H
#define SYSF_ARGS_HELPER_H

#define __comma ,

#define NO_STHD '\0'

#define __ARG_BASE(_name, _shorthand, _type) .name = _name, .shorthand = _shorthand, .type = _type
#define BOOL_ARG(x, title, name, shorthand) x(title, {__ARG_BASE(name, shorthand, ARG_TYPE_BOOLEAN) __comma .inv_set = 0})
#define INV_BOOL_ARG(x, title, name, shorthand) x(title, {__ARG_BASE(name, shorthand, ARG_TYPE_BOOLEAN) __comma .inv_set = 1})
#define INT_ARG(x, title, name, shorthand) x(title, {__ARG_BASE(name, shorthand, ARG_TYPE_INT)})
#define INT_ARG_DEF(x, title, name, shorthand, _default) x(title, {__ARG_BASE(name, shorthand, ARG_TYPE_INT) __comma .value = _default})
#define STRING_ARG(x, title, name, shorthand) x(title, {__ARG_BASE(name, shorthand, ARG_TYPE_STRING)})

#endif