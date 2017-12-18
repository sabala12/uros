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

#define INTERRUPT_GATE		0xE
#define TRAP_GATEE		0xF

#define IDT_ENTRIES_LEN		256

#define gcc_packed		__attribute__ ((packed))

/*
   Interrupt Gate:  Clears IF flag, and restores it on IRET. By this, it ensures the handler won't be interfered by a NMI.
   Trap Gate:       Similar to Interrupt Gates, only that it does not touch the IF flag.
   Task Gate:       ...
 */
typedef void(*idt_handler)(unsigned int vector);

typedef struct idt_flags {
	u8 gate_type:4;
	u8 storage_segment:1;
	u8 dpl:2;
	u8 present:1;
} gcc_packed idt_flags_t;

typedef struct selector {
	u16 offset;
} gcc_packed selector_t;

typedef struct ist { 
	u8 offset:3;
} gcc_packed ist_t;

typedef struct idt_desc {
	u16		offset_1;		// offset bits 0..15
        u16		selector;		// a code segment selector in GDT or LDT
	u8		ist:3;			// bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
	u8		ist_pedding:5;		
        idt_flags_t	flags;			// type and attributes
        u16		offset_2;		// offset bits 16..31
        u32		offset_3;		// offset bits 32..63
        u32		zero;			// reserved
} gcc_packed idt_desc_t;

/* A pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it */
typedef struct idt_register{
	u16 limit;
	u64 base;
} __attribute__((packed)) idt_register_t;

void open_interrupts();
void close_interrupts();
void idt_init();
void idt_assign_handler(idt_handler handler, int entry_num);

#endif //UROS_IDT_H
