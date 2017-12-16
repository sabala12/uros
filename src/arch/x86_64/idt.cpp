#include <arch/x86_64/idt.h>

#include <utils/fmt.h>
#include <drivers/vga.h>

#include <arch/x86_64/core/fault.h>
#include <arch/x86_64/core/stub.h>

#define SET_BIT(n)		(1ULL << n)

#define BITMASK_1_BYTES		(SET_BIT(8)  - 1)
#define BITMASK_2_BYTES		(SET_BIT(16) - 1)
#define BITMASK_4_BYTES		(SET_BIT(32) - 1)

/* An array of pointers to idt handlers. */
idt_handler idt_handler_arr[IDT_ENTRIES_LEN];

/* The actual interrupt descriptor to be loaded into ***.
 * All descriptors goes to fault_handler(), which then invoke
 * the appropriate vector function handler. */
idt_desc_t  idt_desc_arr[IDT_ENTRIES_LEN];

/****************************************/
/***** Default idt descriptor values ****/
type_attr_t idt_type_attr = {

        .gate_type		= INTERRUPT_GATE_TYPE,
        .storage_segment	= NO_STORAGE_SEGMENT,
        .dpl			= DPL_ZERO,
        .present		= PRESENT_ENTRY,
};

selector_t idt_selector = {
	.dpl			= DPL_ZERO,
	.table			= GDT_SELECTOR,
	.offset			= 8,
};

ist_t idt_ist = {
	.offset = 0,
};
/****************************************/

#define idt_set_attr(desc)			do {			\
	(desc)->type_attr	= idt_type_attr;			\
	(desc)->selector	= idt_selector;				\
	(desc)->ist		= idt_ist;				\
} while(0)

#define idt_set_handler(desc, handler)	       do {			\
	u64 handler_		= (u64)handler;				\
	(desc)->offset_1	= (handler_ >> 0 ) & BITMASK_2_BYTES;	\
	(desc)->offset_2	= (handler_ >> 16) & BITMASK_2_BYTES;	\
	(desc)->offset_3	= (handler_ >> 32) & BITMASK_4_BYTES;	\
} while(0)

#define foreach_idt_entry(i_, code_)		do {			\
	i_ = 0;								\
	for (i_ = 0; i_ < IDT_ENTRIES_LEN; i_++) {			\
		code_							\
	}								\
} while(0)


interrupt_stub(fault_handler);

#define stub_by_vec(func, vec) asm ("push %0; jmp "#func::"i"((char)(vec)));

extern u8 idt_handlers_start[], idt_handlers_stop[];
void idt_handlers_section()
{
	asm volatile("idt_handlers_start:");
#define CI(i) stub_by_vec(fault_handler, i)
	CI(0);
	asm volatile("idt_handlers_stop:");
#define CI1(x) CI(x);  CI(x+1);
#define CI2(x) CI1(x); CI1(x+2);
#define CI3(x) CI2(x); CI2(x+4);
#define CI4(x) CI3(x); CI3(x+8);
#define CI5(x) CI4(x); CI4(x+16);
#define CI6(x) CI5(x); CI5(x+32);
#define CI7(x) CI6(x); CI6(x+64);
#define CI8(x) CI7(x); CI7(x+128);
	CI8(1);
}

void idt_assign_handler(idt_handler handler, int entry_num)
{
	idt_set_handler(&idt_desc_arr[entry_num], handler);
}

void idt_init()
{
	(void)idt_handlers_section;

	int i;
	int handler_size;

	handler_size = int((u64)idt_handlers_start - (u64)idt_handlers_stop);
	foreach_idt_entry(i, {
		/* Find the nth handler. */
		idt_handler handler = (idt_handler)(idt_handlers_start + (i * handler_size));

		/* Init idt entry, */
		idt_set_attr(&idt_desc_arr[i]);
		idt_set_handler(&idt_desc_arr[i], handler);;
	});
}
