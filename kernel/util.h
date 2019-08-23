#ifndef UTIL_H
#define UTIL_H

#include "../cpu/types.h"

void memory_copy(char *source, char *dest, int nbytes);
void memory_copy_int(int * source, int * dest, int no_bytes);
/* Converts integer to ASCII string
 *  n - integer to convert
 *  str - table that will hold ASCII string
 */
void int_to_ascii(int n, char str[]);

#endif
