//
// Created by eran on 09/09/17.
//

#ifndef UROS_IDT_H
#define UROS_IDT_H

#include <stddef.h>
#include <stdint.h>

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

#define SYSTEM_IDT_LEN  32

/*
   Interrupt Gate:  Clears IF flag, and restores it on IRET. By this, it ensures the handler won't be interfered by a NMI.
   Trap Gate:       Similar to Interrupt Gates, only that it does not touch the IF flag.
   Task Gate:       ...
 */
namespace idt
{
    typedef void(*idt_handler_ptr)(unsigned int vector);

    typedef struct type_attr
    {
        u8 gate_type:4;
        u8 storage_segment:1;
        u8 dpl:2;
        u8 present:1;

    } type_attr_t;

    typedef struct selector
    {
        u16 dpl:2;
        u16 table:1;
        u16 offset:13;

    } selector_t;

    typedef struct ist
    {
        u8 offset:3;

    } ist_t;

    typedef struct idt_desc
    {
        u16 offset_1;           // offset bits 0..15
        selector_t selector;    // a code segment selector in GDT or LDT
        ist_t ist;              // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
        type_attr_t type_attr;  // type and attributes
        u16 offset_2;           // offset bits 16..31
        u32 offset_3;           // offset bits 32..63
        u32 zero;               // reserved

    } __attribute__((packed)) idt_desc_t;

    /* Stores all system interrupt handlers. */
    idt_desc_t idt[SYSTEM_IDT_LEN];

    /* Create an idt type_attr. */
    type_attr_t create_idt_type_attr(u8 gate_type, u8 storage_segment, u8 dpl, u8 present)
    {
        type_attr_t type_attr;
        type_attr.gate_type         = gate_type & 0xF;
        type_attr.storage_segment   = storage_segment & 0x1;
        type_attr.dpl               = dpl & 0x3;
        type_attr.present           = present & 0x1;

        return type_attr;
    }

    /* Create an idt selector. */
    selector_t create_idt_selector(u8 dpl, u8 table, u16 offset)
    {
        selector_t selector;
        selector.dpl = dpl;
        selector.table = dpl;
        selector.offset = dpl;

        return selector;
    }

    /* Create a new interrupt gate type attr. */
    type_attr_t create_interrupt_gate_type_attr()
    {
        return create_idt_type_attr(INTERRUPT_GATE_TYPE, NO_STORAGE_SEGMENT, DPL_ZERO, PRESENT_ENTRY);
    }

    /* Create a new interrupt gate selector. */
    selector_t create_interrupt_gate_selector()
    {
        //TODO::Check if the offset is in bytes, or used as an index.
        //TODO::Check how to import(by extern declaration) inner labels from asm.
        return create_idt_selector(DPL_ZERO, GDT_SELECTOR, 8);
    }

    /* Initialize an idt descriptor. */
    void init_idt_desc(idt_desc_t* idt_desc, idt_handler_ptr handler, type_attr_t type_attr, selector_t selector, ist_t ist)
    {
        idt_desc->offset_1 = ((u64)handler >> 0)  & 0xFFFF;
        idt_desc->offset_2 = ((u64)handler >> 16) & 0xFFFF;
        idt_desc->offset_3 = ((u64)handler >> 32) & 0xFFFFFFFF;
        idt_desc->type_attr = type_attr;
        idt_desc->selector = selector;
        idt_desc->ist = ist;
    }

    void create_system_idt_entry(int vector, idt_handler_ptr idt_handler)
    {
        //TODO::Check what is ist(Interrupt stack pointer) is used for in long mode.
        type_attr_t type_attr = create_interrupt_gate_type_attr();
        selector_t selector   = create_interrupt_gate_selector();
        ist_t ist;
        ist.offset = 0;
        idt_desc_t* idt_desc  = &idt[vector];
        init_idt_desc(idt_desc, idt_handler, type_attr, selector, ist);
    }

    idt_handler_ptr idt_handlers[SYSTEM_IDT_LEN];
    
    void setup_idt()
    {
        for (int i = 0; i < SYSTEM_IDT_LEN; i++) {
	    create_system_idt_entry(i, idt_handlers[i]);
        }
    }
}

#endif //UROS_IDT_H
