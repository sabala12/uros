global stack_top_0
global stack_top_1
global stack_top_2

; declare page table
section .bss
; ring 0 stack
stack_bottom_0:
    resb 4096 * 4
stack_top_0:

; ring 1 stack
stack_bottom_1:
    resb 4096 * 4
stack_top_1:

; ring 2 stack
stack_bottom_2:
    resb 4096 * 4
stack_top_2: