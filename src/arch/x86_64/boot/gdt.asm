global p4_table
global p3_table
global p2_table
global gdt64.pointer
global gdt64.code

; declare gdt & segment selectors
section .rodata
gdt64:
    dq 0                                                    ; reserved
.code: equ $ - gdt64
    ;   executable     type        present     64-bit seg
    dq  (1<<43)      | (1<<44)   | (1<<47)   | (1<<53)      ; code segment
.pointer:
    dw $ - gdt64 - 1                                        ; gdt size
    dq gdt64                                                ; gdt address