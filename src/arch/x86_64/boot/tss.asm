extern stack_top_0
extern stack_top_1
extern stack_top_2

; declare page table
section .bss
; ring 0 stack
ist_bottom_1:
    resb 4096 * 4
is_top_t:

; declare tss
section .rodata
tss:
    dd 0						    ; reserved
    dq stack_top_0					    ; rsp ring 0
    dq stack_top_1					    ; rsp ring 1
    dq stack_top_2					    ; rsp ring 2
    dq 0						    ; reserved
.ist:
    dq is_top_t						    ; ist_1
    dq 0						    ; ist_2
    dq 0						    ; ist_3
    dq 0						    ; ist_4
    dq 0						    ; ist_5
    dq 0						    ; ist_6
    dq 0						    ; ist_7