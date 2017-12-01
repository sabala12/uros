//
// Created by eran on 09/09/17.
//

#ifndef UROS_IDT_H
#define UROS_IDT_H

#include <libc/types.h>

#define NO_STORAGE_SEGMENT      0
#define PRESENT_ENTRY           1

#define GDT_SELECTOR            0
#define LDT_SELECTOR            1

#define DPL_ZERO                0
#define DPL_ONE                 1
#define DPL_TWO                 2
#define DPL_THREE               3

#define INTERRUPT_GATE_TYPE    0xE
#define INTERRUPT_TRAP_TYPE    0xF

#define IDT_ENTRIES_LEN		32

/*
   Interrupt Gate:  Clears IF flag, and restores it on IRET. By this, it ensures the handler won't be interfered by a NMI.
   Trap Gate:       Similar to Interrupt Gates, only that it does not touch the IF flag.
   Task Gate:       ...
 */
typedef void(*idt_handler)(unsigned int vector);

typedef struct type_attr {
	u8 gate_type:4;
	u8 storage_segment:1;
	u8 dpl:2;
	u8 present:1;
} type_attr_t;

typedef struct selector {
	u16 dpl:2;
	u16 table:1;
	u16 offset:13;
} selector_t;

typedef struct ist { 
	u8 offset:3;
} ist_t;

typedef struct idt_desc {
	u16		offset_1;           // offset bits 0..15
        selector_t	selector;	    // a code segment selector in GDT or LDT
        ist_t		ist;                // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
        type_attr_t	type_attr;  	    // type and attributes
        u16		offset_2;           // offset bits 16..31
        u32		offset_3;           // offset bits 32..63
        u32		zero;               // reserved
} __attribute__((packed)) idt_desc_t;

void idt_setup_table();
void idt_assign_handler(idt_handler handler, int entry_num);

#endif //UROS_IDT_H
