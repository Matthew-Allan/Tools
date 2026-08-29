#ifndef SYSF_CONCAT_H
#define SYSF_CONCAT_H

#define _CONCAT(a, b) a ## b
#define CONCAT(a, b) _CONCAT(a, b)

#endif