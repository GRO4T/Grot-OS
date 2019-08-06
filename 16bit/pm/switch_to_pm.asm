[bits 16]
; Switch to protected mode
switch_to_pm:
   cli

lgdt [gdt_descriptor]
; Load our global descriptor table , which defines
; the protected mode segments ( e.g. for code and data )
mov eax, cr0
or eax , 0x1
mov cr0 , eax
; To make the switch to protected mode , we set
; the first bit of CR0 , a control register
jmp CODE_SEG:init_pm

[ bits 32]
; Initialise registers and the stack once in PM.
init_pm :
mov ax, DATA_SEG
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax
; Now in PM , our old segments are meaningless ,
; so we point our segment registers to the
; data selector we defined in our GDT
mov ebp , 0x90000
mov esp, ebp
call BEGIN_PM
; Update our stack position so it is right
; at the top of the free space.
; Finally , call some well - known label
