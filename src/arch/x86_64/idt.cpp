#include <arch/x86_64/idt.h>

#include <utils/fmt.h>
#include <drivers/vga.h>

#include <arch/x86_64/core/fault.h>
#include <arch/x86_64/core/stub.h>

#include <utils/fmt.h>

#define SET_BIT(n)		(1ULL << n)

#define BITMASK_1_BYTES		(SET_BIT(8)  - 1)
#define BITMASK_2_BYTES		(SET_BIT(16) - 1)
#define BITMASK_4_BYTES		(SET_BIT(32) - 1)

#define gcc_align(n)		__attribute__ ((aligned (n)))

/* An array of pointers to idt handlers. */
idt_handler idt_handler_arr[IDT_ENTRIES_LEN] gcc_align(16);

/* The actual interrupt descriptor to be loaded into ***.
 * All entries point to fault_handler(), which then invoke the appropriate vector handler. */
idt_desc_t  idt_desc_arr[IDT_ENTRIES_LEN] gcc_align(16);

idt_register_t idt_reg;

/***** Default idt descriptor values ****/
//TODO:: gdt64_code value is wrong.
//extern u64 gdt64_code;
//gdt64_code has wired value.
//u16 idt_selector = gdt64_code;
u16 idt_selector = 0x8;

constexpr ist_t idt_ist = { 
	.offset = 0, 
};

constexpr idt_flags_t idt_flags = {

        .gate_type		= INTERRUPT_GATE,
        .storage_segment	= NO_STORAGE_SEGMENT,
        .dpl			= DPL_ZERO,
        .present		= PRESENT_ENTRY,
};

/*
 * 31							   0
 *  -------------------------------------------------------
 * |			   reserved			   | 12:15
 *  -------------------------------------------------------
 *
 * 31							   0 
 *  -------------------------------------------------------
 * |			   handler 			   |  8:12
 *  -------------------------------------------------------
 *
 * 31		          15     14  13      12    8   3   0 
 *  -------------------------------------------------------
 * |	   handler        |present|dpl|storage|type|..0|ist|   4:7
 *  -------------------------------------------------------
 *
 * 31			      16			   0
 *  -------------------------------------------------------
 * |	  cs selector	      |         handler 	   |   0:3
 *  -------------------------------------------------------
 */

#define idt_set_attr(desc)			do {			\
	(desc)->flags	= idt_flags;					\
	(desc)->selector	= idt_selector;				\
	(desc)->ist		= 0;					\
	(desc)->ist_pedding	= 0;					\
	(desc)->zero		= 0;					\
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


extern u8 idt_handlers_start[], idt_handlers_stop[];
void idt_handlers_section()
{
	asm volatile("idt_handlers_start:");
#define CI(vec) interrupt_stub(vec)
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
	(void)idt_handlers_section;
	idt_set_handler(&idt_desc_arr[entry_num], handler);
}

void idt_load_register()
{
	idt_reg.base = (u64)&idt_desc_arr;
	idt_reg.limit = IDT_ENTRIES_LEN * sizeof(idt_desc_t) - 1;
	asm volatile("lidt %0":: "m"(idt_reg));
}

void open_interrupts()
{
	asm volatile("sti"::);
}

void close_interrupts()
{
	asm volatile("cli"::);
}

void idt_init()
{
	int i;
	int handler_size;

	/* Calc the size of idt handler wraper in order to iterate them. */
	handler_size = int((u64)idt_handlers_start - (u64)idt_handlers_stop);
	foreach_idt_entry(i, {
		/* Find the nth handler. */
		idt_handler handler = (idt_handler)(idt_handlers_start + (i * handler_size));

		/* Init idt entry, */
		idt_set_attr(&idt_desc_arr[i]);
		idt_set_handler(&idt_desc_arr[i], handler);;
	});

	idt_load_register();
	//open_interrupts();
}
