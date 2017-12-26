#ifndef UROS_FAULT_H
#define UROS_FAULT_H

#include <arch/x86_64/core/stub.h>
#include <utils/fmt.h>

#define no_mangling	extern "C"

struct fault_regs {
    uint64_t rbp;
    uint64_t error_no;
    uint64_t error_code;
    uint64_t rip;
    uint64_t rflags;
    uint64_t cs;
    uint64_t rsp;
    uint64_t ss;
} __attribute__((packed));

/* Generic exception stub.
 * Handle vector 0 - 31, except for those with real error code. */
#define long_exception_stub(vec)						\
	asm (									\
	".section .text"			"\n"				\
	"	push $0"			"\n"				\
	"	push %0"			"\n"				\
	"	mov %%rsp, %%rdi"		"\n"				\
	"	call fault_handler"		"\n"				\
	"	add $16, %%rsp"			"\n"				\
	"	iretq"				"\n"				\
	".previous"				"\n"				\
	::"i"(vec))

/* Generic exception stub.
 * A special case for vector: 8, 10, 11, 12, 13, 14.
 * The cpu pushes a real error code for these vectors. */
#define short_exception_stub(vec)						\
	asm (									\
	".section .text"			"\n"				\
	"	push %0"			"\n"				\
	"	mov %%rsp, %%rdi"		"\n"				\
	"	call fault_handler"		"\n"				\
	"	add $8, %%rsp"			"\n"				\
	"	iretq"				"\n"				\
	".previous"								\
	::"i"(vec))

	asm (
	".section .text\n" 
	"	common_interrupt_stub:"		"\n"
	"	mov %rsp, %rdi"			"\n"
	"	call fault_handler"		"\n"
	"	add $8, %rsp"			"\n"
	"	iretq"				"\n"
	".previous");
	

no_mangling void fault_handler(struct fault_regs* regs)
{
	u64 rsp;
	asm volatile ("mov %%rsp, %0":"=r"(rsp)::);
	print("hello from handler rsp=0x%x", rsp);
} 

#endif // UROS_FAULT_H
