extern stack_top_0
extern stack_top_1
extern stack_top_2

; declare page table
section .bss

; interrupt stack table
ist_bottom_1:
    resb 4096 * 4
ist_top_1:

ist_bottom_2:
    resb 4096 * 4
ist_top_2:

ist_bottom_3:
    resb 4096 * 4
ist_top_3:

ist_bottom_4:
    resb 4096 * 4
ist_top_4:

ist_bottom_5:
    resb 4096 * 4
ist_top_5:

ist_bottom_6:
    resb 4096 * 4
ist_top_6:

ist_bottom_7:
    resb 4096 * 4
ist_top_7:

; declare tss
section .rodata
tss:
    dd 0						    ; reserved
    dq stack_top_0					    ; rsp ring 0
    dq stack_top_1					    ; rsp ring 1
    dq stack_top_2					    ; rsp ring 2
    dq 0						    ; reserved
.ist:
    dq ist_top_1
    dq ist_top_2
    dq ist_top_3
    dq ist_top_4
    dq ist_top_5
    dq ist_top_6
    dq ist_top_7