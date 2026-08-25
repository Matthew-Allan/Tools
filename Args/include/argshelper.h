#ifndef SYSF_ARGS_HELPER_H
#define SYSF_ARGS_HELPER_H

#define __comma ,

#define NO_STHD '\0'

#define BOOL_ARG(x, _title, _name, _shorthand) x(_title, {.name = _name __comma .shorthand = _shorthand __comma .type = ARG_TYPE_BOOLEAN __comma .inv_set = 0})
#define INV_BOOL_ARG(x, _title, _name, _shorthand) x(_title, {.name = _name __comma .shorthand = _shorthand __comma .type = ARG_TYPE_BOOLEAN __comma .inv_set = 1})
#define INT_ARG(x, _title, _name, _shorthand) x(_title, {.name = _name __comma .shorthand = _shorthand __comma .type = ARG_TYPE_INT})
#define INT_ARG_DEF(x, _title, _name, _shorthand, _default) x(_title, {.name = _name __comma .shorthand = _shorthand __comma .type = ARG_TYPE_INT __comma .value = _default})
#define STRING_ARG(x, _title, _name, _shorthand) x(_title, {.name = _name __comma .shorthand = _shorthand __comma .type = ARG_TYPE_STRING})

#endif