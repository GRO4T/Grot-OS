#include "keyboard.h"
#include "ports.h"
#include "screen.h"
#include "../kernel/util.h"

#define KEYBOARD_DATA_PORT 0x60

unsigned char read_scan_code(){
    return port_byte_in(KEYBOARD_DATA_PORT);
}

void handle_keyboard(registers_t r){
    char s[3];
    int scancode = read_scan_code();
    int_to_ascii(scancode, s);
    kprint(s);
}
