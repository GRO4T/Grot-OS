#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
//#include "../test/screen_unit_test.h"
#include "../drivers/keyboard.h"

#define NULL 0

void main () {
    clear_screen();
    isr_install();

    asm volatile("sti");
    register_interrupt_handler(33, handle_keyboard);
    /* Test the interrupts */
    //__asm__ __volatile__("int $2");
    //__asm__ __volatile__("int $3");
    //__asm__ __volatile__("int $33");
    kprint("waiting...");
}
