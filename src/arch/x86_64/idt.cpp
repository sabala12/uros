#include <arch/x86_64/idt.h>
#include <utils/fmt.h>
#include <drivers/vga.h>

#define __uros_read_mostly__

#define SET_BIT(n)		(1ULL << n)

#define BITMASK_1_BYTES		(SET_BIT(8)  - 1)
#define BITMASK_2_BYTES		(SET_BIT(16) - 1)
#define BITMASK_4_BYTES		(SET_BIT(32) - 1)

#define IDT_SET_DEFAULT_ATTRS(desc)		do {			\
	(desc)->type_attr	= idt_type_attr;			\
	(desc)->selector	= idt_selector;				\
	(desc)->ist		= idt_ist;				\
} while(0)

#define IDT_ASSIGN_HANDLER(desc, handler)	       do {		\
	u64 handler_		= (u64)handler;				\
	(desc)->offset_1	= (handler_ >> 0 ) & BITMASK_2_BYTES;	\
	(desc)->offset_2	= (handler_ >> 16) & BITMASK_2_BYTES;	\
	(desc)->offset_3	= (handler_ >> 32) & BITMASK_4_BYTES;	\
} while(0)

#define FOREACH_IDT_ENTRY(i_, code_)		do {			\
	i_ = 0;								\
	for (i_ = 0; i_ < IDT_ENTRIES_LEN; i_++) {			\
		code_							\
	}								\
} while(0)

idt_desc_t  idt_desc_arr[IDT_ENTRIES_LEN];
idt_handler idt_handler_arr[IDT_ENTRIES_LEN];

type_attr_t __uros_read_mostly__ idt_type_attr = {

        .gate_type		= INTERRUPT_GATE_TYPE,
        .storage_segment	= NO_STORAGE_SEGMENT,
        .dpl			= DPL_ZERO,
        .present		= PRESENT_ENTRY,
};

selector_t __uros_read_mostly__ idt_selector = {
	.dpl			= DPL_ZERO,
	.table			= GDT_SELECTOR,
	.offset			= 8,
};

ist_t __uros_read_mostly__ idt_ist = {
	.offset = 0,
};

static void idt_init_entries()
{
	int i;
	FOREACH_IDT_ENTRY(i, {
		IDT_SET_DEFAULT_ATTRS(&idt_desc_arr[i]);
	});
}

static void idt_assign_handlers()
{
	int i;
	FOREACH_IDT_ENTRY(i, {
		IDT_ASSIGN_HANDLER(&idt_desc_arr[i], idt_handler_arr[i]);
	});
}

void idt_assign_handler(idt_handler handler, int entry_num)
{
	IDT_ASSIGN_HANDLER(&idt_desc_arr[entry_num], handler);
}

void idt_setup_table()
{
	idt_init_entries();
	idt_assign_handlers();
}
