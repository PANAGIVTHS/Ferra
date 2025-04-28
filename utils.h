#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define allocError(ptr, func) \
do { \
if ((ptr) == NULL) { \
fprintf(stderr, "Malloc - Allocation Error in %s\n", func); \
return NULL; \
} \
} while (0)

#endif
