#include "idt.h"
#include "../kernel/util.h"

#define NULL 0

void set_idt_gate(int n, u32 handler){
   idt[n].low_offset = low_16(handler);
   idt[n].sel = KERNEL_CS;
   idt[n].always0 = 0;
   idt[n].flags = 0x8E; // 1000 1110
   idt[n].high_offset = high_16(handler);
}

void set_idt(){
    idt_reg.base = (u32)&idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg)); // notice lidtl not lidt! it's not NASM assembly
    /* volatile keyword
     *      tells the compiler that the assembly has side-effects that the output, input and clobber lists do
     *      not represent. For example, use this keyword with inline assembly code that sets the value of a
     *      system register.
     *
     *      The compiler assumes that any inline assembly with no output operands is volatile. However it is
     *      recommended to still use it for clarity and to avoid a behaviour change if an output is added later.
     */
}
