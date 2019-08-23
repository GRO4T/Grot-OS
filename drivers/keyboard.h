#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/isr.h"

unsigned char read_scan_code(void);
void handle_keyboard(registers_t r);

#endif
