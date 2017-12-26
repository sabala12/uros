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
		code_;							\
	}								\
} while(0)

#define foreach_idt_exception(i_, code_)	do {			\
	i_ = 0;							\
	for (i_ = 0; i_ < IDT_EXCEPTIONS_LEN; i_++) {			\
		code_;							\
	}								\
} while(0)

void idt_assign_handler(idt_handler handler, int entry_num)
{
	idt_set_handler(&idt_desc_arr[entry_num], handler);
}

void idt_load_register()
{
	idt_reg.base = (u64)&idt_desc_arr;
	idt_reg.limit = IDT_ENTRIES_LEN * sizeof(idt_desc_t) - 1;
	asm volatile("lidt %0":: "m"(idt_reg));
}

extern u8 idt_long_exceptions_start[], idt_long_exceptions_end[];
extern u8 idt_short_exceptions_start[], idt_short_exceptions_end[];
no_mangling void idt_exceptions()
{
	asm volatile("idt_long_exceptions_start:");
#define CI(vec) long_exception_stub(vec)
	CI(0);
	asm volatile("idt_long_exceptions_end:");
#define CI1(x) CI(x);  CI(x+1);
#define CI2(x) CI1(x); CI1(x+2);
#define CI3(x) CI2(x); CI2(x+4);
#define CI4(x) CI3(x); CI3(x+8);
#define CI5(x) CI4(x); CI4(x+16);
	CI5(1);

	asm volatile("idt_short_exceptions_start:");
	short_exception_stub(SET_BIT(8));
	asm volatile("idt_short_exceptions_end:");
	short_exception_stub(SET_BIT(10));
	short_exception_stub(SET_BIT(11));
	short_exception_stub(SET_BIT(12));
	short_exception_stub(SET_BIT(13));
	short_exception_stub(SET_BIT(14));
}

#define idt_calc_handler_size(name)		\
	((u64)name ## _end - (u64)name ## _start)

#define idt_get_handler_i(name, index, size)	\
	(idt_handler)(name ## _start + (index * size))

/* At vectors: 8, 10, 11, 12, 13, 14, the cpu push an error code on the stack.
 * For this vector assign a handler without dummy error code push. */
void idt_modify_special_exceptions()
{
	int i;
	int size;
	idt_handler handler;
	const u64 real_error_code_exceptions =  SET_BIT(8) & 
						SET_BIT(10) &
						SET_BIT(11) &
						SET_BIT(12) &
						SET_BIT(13) &
						SET_BIT(14);

	size = idt_calc_handler_size(idt_short_exceptions);
	foreach_idt_exception(i, {
		if (real_error_code_exceptions & SET_BIT(i)) {
			/* Assign the short exception handler (i.e no dummy error code push). */
			handler = idt_get_handler_i(idt_short_exceptions, i, size);
			idt_set_handler(&idt_desc_arr[i], handler);
		}
	});
}

void idt_load_exceptions()
{
	int i;
	int size;
	idt_handler handler;

	size = idt_calc_handler_size(idt_long_exceptions);
	foreach_idt_entry(i, {

		/* Init the nth idt exception entry. */
		handler = idt_get_handler_i(idt_long_exceptions, i, size);
		idt_set_attr(&idt_desc_arr[i]);
		idt_set_handler(&idt_desc_arr[i], handler);;
	});

	idt_modify_special_exceptions();
}

void idt_init()
{
	idt_load_exceptions();
	idt_load_register();
}
