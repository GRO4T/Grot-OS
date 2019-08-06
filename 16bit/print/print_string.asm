print_string: ; asm routine for printing strings
    pusha ; push registers on stack (to save initial state of the program)
    mov ah, 0x0e ; tty mode (prints everything that is in al?)

print_char:
    mov al, [bx] ; bx points to our string
    cmp al, 0
    je end_of_print
    add bx, 1
    int 0x10 ; interrupt for printing
    jmp print_char

end_of_print:
    popa
    ret
