//
// Created by eran on 09/09/17.
//

#ifndef UROS_IDT_H
#define UROS_IDT_H

#include <stddef.h>
#include <stdint.h>

#define SYSTEM_IDT_LEN  32

#define BIT_MASK(n)

namespace idt {
    typedef void(*idt_handler_prt)(unsigned int vector);

    typedef struct type_attr {
        u8 gate_type:4;
        u8 storage_segment:1;
        u8 dpl:2;
        u8 present:1;

    } type_attr_t;

    typedef struct idt_desc {
        u16 offset_1;  // offset bits 0..15
        u16 selector;  // a code segment selector in GDT or LDT
        u8 ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
        type_attr_t type_attr; // type and attributes
        u16 offset_2;  // offset bits 16..31
        u32 offset_3;  // offset bits 32..63
        u32 zero;      // reserved

    } __attribute__((packed)) idt_desc_t;

    type_attr_t create_idt_type_attr(u8 gate_type, u8 storage_segment, u8 dpl, u8 present) {
        type_attr_t type_attr;
        type_attr.gate_type = gate_type & 0xF;
        type_attr.storage_segment = storage_segment & 0x1;
        type_attr.dpl = storage_segment & 0x3;
        type_attr.present = storage_segment & 0x1;

        return type_attr;
    }

    void set_idt_desc(idt_desc_t &idt_desc, u64 address, type_attr_t type_attr, u16 selector, u8 ist) {
        idt_desc.offset_1 = (address >> 0) & 0xFFFF;
        idt_desc.offset_2 = (address >> 16) & 0xFFFF;
        idt_desc.offset_3 = (address >> 32) & 0xFFFFFFFF;
        idt_desc.type_attr = type_attr;
        idt_desc.ist = ist;
    }

    idt_desc_t idt[SYSTEM_IDT_LEN];

    template<int vector>
    void create_idt_entry(idt_handler_prt idt_handler) {
        //idt_desc_t idt_desc;
        //set_idt_desc(idt_desc, idt_handler,)
    }

    void setup_idt() {
        for (int i = 0; i < SYSTEM_IDT_LEN; i++) {
        }
    }
}

#endif //UROS_IDT_H
