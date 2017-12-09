global p4_table
global p3_table
global p2_table
global gdt64_pointer
global gdt64_code

; A GDT always starts with a 0-entry and contains an
; arbitrary number of segment entries afterwards. 
; A 64-bit entry has the following format:

; Bit(s)	Name		Meaning
; 0-41		ignored		-------
; 42		conforming	The current privilege level can be higher
;				than the specified level for code segments
; 43		executable	1: code segment
;				0: data segment
; 44		descriptor	1: code & data segments
;		type	
; 45-46		privilege	CPL: 0 for kernel, 3 for user
; 47		present		1: valid selector
; 48-52		ignored		-------
; 53		64-bit		1: 64-bit code segment
; 54		32-bit		0: 64-bit segments
; 55-63		ignored		------- */
;
; A data segment is not necessary in 64-bit mode */

; ************* GDT *************
section .rodata
gdt64:
    dq 0                                                    ; reserved
gdt64_code: equ $ - gdt64
    ;   executable     type        present     64-bit seg
    dq  (1<<43)      | (1<<44)   | (1<<47)   | (1<<53)      ; code segment
gdt64_pointer:
    dw $ - gdt64 - 1                                        ; gdt size
    dq gdt64                                                ; gdt address
