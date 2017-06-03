global long_mode_start
extern main

section .text
bits 64
long_mode_start:
    ; load 0 into all data segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; push multiboot_header address to main
    ; pop rbx

    call main

    hlt
