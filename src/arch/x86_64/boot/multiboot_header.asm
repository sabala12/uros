%define flag_a  (0 << 0)
%define flag_b  (0 << 1)

section .multiboot_header
header_start:
    dd 0xe85250d6							; magic number (multiboot 2)
    dd flag_a | flag_b							; architecture 0 (protected mode i386)
    dd header_end - header_start					; header length
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))	; checksum

    ; insert optional multiboot tags here

    ; required end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
header_end:
