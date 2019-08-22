#ifndef UTIL_H
#define UTIL_H

#include "../cpu/types.h"

void memory_copy(char *source, char *dest, int nbytes);
void memory_copy_int(int * source, int * dest, int no_bytes);
void int_to_ascii(int n, char str[]);

#endif
