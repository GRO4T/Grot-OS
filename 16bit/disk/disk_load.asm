; load DH sectors to ES : BX from drive DL
;disk_load :
;push dx
;; Store DX on stack so later we can recall
;; how many sectors were request to be read ,
;    ; even if it is altered in the meantime
;    cli
;
;    mov ah , 0x02
;    ; BIOS read sector function
;    mov al , dh
;    ; Read DH sectors
;    mov ch , 0x00
;    ; Select cylinder 0
;    mov dh , 0x00
;    ; Select head 0
;    mov cl , 0x02
;    ; Start reading from second sector ( i.e.
;            ; after the boot sector )
;    int 0x13
;    ; BIOS interrupt
;    jc disk_error_carry
;    ; Jump if error ( i.e. carry flag set )
;    pop dx
;    ; Restore DX from the stack
;    cmp dh , al
;    ; if AL ( sectors read ) != DH ( sectors expected )
;    jne disk_error
;    ; display error message
;    ret

disk_load:
  mov  [SECTORS], dh
  mov  ch, 0x00      ;C=0 cylinder
  mov  dh, 0x00      ;H=0 head
  mov  cl, 0x02      ;S=2 sectors to read
 next_group:
  mov  di, 5         ;Max 5 tries
 again: 
  mov  ah, 0x02      ;Read sectors
  mov  al, [SECTORS]
  int  0x13
  jc   maybe_retry
  sub  [SECTORS], al ;Remaining sectors
  jz  ready
  mov  cl, 0x01      ;Always sector 1
  xor  dh, 1         ;Next head on diskette!
  jnz  next_group
  inc  ch            ;Next cylinder
  jmp  next_group
 maybe_retry:
  mov  ah, 0x00      ;Reset diskdrive
  int  0x13
  dec  di
  jnz  again
  jmp  disk_error
 ready:
  ret

    disk_error :
    mov bx , DISK_ERROR_MSG
    call print_string
    jmp $
    ; Variables
    DISK_ERROR_MSG:
    db " Disk read error ! " , 0

; Global variables

SECTORS db 0


