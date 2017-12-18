#ifndef UROS_FAULT_H
#define UROS_FAULT_H

#include <arch/x86_64/core/stub.h>
#include <utils/fmt.h>

#define no_mangling	extern "C"

//"	mov  %%rsp, %%rdi"	"\n"							
//"	push %0 "		"\n"							
//"	add  0x8, %%rsp"	"\n"							
#define interrupt_stub(vec)						\
	asm (								\
	".section .text\n"						\
	"	call fault_handler"	"\n"				\
	"	iretq\n"						\
	".previous\n"							\
	::"i"(vec))

no_mangling void fault_handler()
{
	print("hello from handler");
} 

#endif // UROS_FAULT_H
