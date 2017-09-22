global p4_table
global p3_table
global p2_table
global gdt64_pointer
global gdt64_code

;TODO::Check the segments descriptors are correct.
;TODO::Check which segments descriptors should be used init in long mode.

;gdt & segment selectors
section .rodata
gdt64:
    dq 0                                                    ; reserved
gdt64_code: equ $ - gdt64
    ;   executable     type        present     64-bit seg
    dq  (1<<43)      | (1<<44)   | (1<<47)   | (1<<53)      ; code segment
gdt64_pointer:
    dw $ - gdt64 - 1                                        ; gdt size
    dq gdt64                                                ; gdt address