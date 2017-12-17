#ifndef UROS_STUB_H
#define UROS_STUB_H

#define ASM_DECLARE_FUNC(__func)		\
	".globl   " #__func "\n"		\
	".type    " #__func ", @function\n"	\
	#__func ":\n"

#define PUSH_ALL_REGS()			\
	"	mov %r15,112(%rsp)\n"	\
	"	mov %r14,104(%rsp)\n"	\
	"	mov %r13,96(%rsp)\n"	\
	"	mov %r12,88(%rsp)\n"	\
	"	mov %r11,80(%rsp)\n"	\
	"	mov %r10,72(%rsp)\n"	\
	"	mov %r9,64(%rsp)\n"	\
	"	mov %r8,56(%rsp)\n"	\
	"	mov %rdi,48(%rsp)\n"	\
	"	mov %rsi,40(%rsp)\n"	\
	"	mov %rbp,32(%rsp)\n"	\
	"	mov %rbx,24(%rsp)\n"	\
	"	mov %rdx,16(%rsp)\n"	\
	"	mov %rcx,8(%rsp)\n"	\
	"	mov %rax,0(%rsp)\n"	\

#define POP_ALL_REGS()			\
	"	mov 112(%rsp),%r15\n"	\
	"	mov 104(%rsp),%r14\n"	\
	"	mov 96(%rsp),%r13\n"	\
	"	mov 88(%rsp),%r12\n"	\
	"	mov 80(%rsp),%r11\n"	\
	"	mov 72(%rsp),%r10\n"	\
	"	mov 64(%rsp),%r9\n"	\
	"	mov 56(%rsp),%r8\n"	\
	"	mov 48(%rsp),%rdi\n"	\
	"	mov 40(%rsp),%rsi\n"	\
	"	mov 32(%rsp),%rbp\n"	\
	"	mov 24(%rsp),%rbx\n"	\
	"	mov 16(%rsp),%rdx\n"	\
	"	mov 8(%rsp),%rcx\n"	\
	"	mov 0(%rsp),%rax\n"	\

#define execption_stub(func);

#define interrupt_stub(func)		\
	void func ## _stub();		\
	void func() __attribute__((used));\
	asm (				\
	".section .text\n" 		\
	ASM_DECLARE_FUNC(func ## _stub)	\
	"	sub $120,%rsp\n"   	\
	PUSH_ALL_REGS()			\
					\
	/* the x86_64 function calling convention uses rdi as a function's first	*/	\
	/* argument. we copy rsp to rdi and call the interrupt handler (see		*/	\
	/* gp_fault() or interrupt_func()) whose first argument is a pointer to		*/	\
	/* "struct regs_t" thus the regs object is actually located on the stack.	*/	\
	"	mov %rsp,%rdi\n"	\
	"	cld\n"			\
	"	call	" #func "\n"	\
	POP_ALL_REGS()			\
					\
	/* notice that the stack is advanced by 128 bytes and previously (at the	*/	\
	/* beginning of the function) it was decreased by only 120 bytes. this		*/	\
	/* anomaly compensates for an extra word (error-code), pushed on 		*/	\
	/* the stack by some cpu faults, before doing an iret which doesn't expect	*/	\
	/* this extra word on the stack..						*/	\
	/* there are some exceptions that don't have an error code pushed on the	*/	\
	/* stack and instead we push an extra word on the stack before calling the	*/	\
	/* exception handler (see stub_by_vec()).					*/	\
	"	add $128,%rsp\n"	\
	#func "_iret:\n"	    	\
	"	iretq\n"		\
	".previous\n")

#define interrupt_stub_2(func)		\
	void func ## _stub();		\
	void func() __attribute__((used));\
	asm (				\
	".section .text\n" 		\
	ASM_DECLARE_FUNC(func ## _stub)	\
					\
	/* the x86_64 function calling convention uses rdi as a function's first	*/	\
	/* argument. we copy rsp to rdi and call the interrupt handler (see		*/	\
	/* gp_fault() or interrupt_func()) whose first argument is a pointer to		*/	\
	/* "struct regs_t" thus the regs object is actually located on the stack.	*/	\
	"	mov %rsp,%rdi\n"	\
	"	call	" #func "\n"	\
	#func "_iret:\n"	    	\
	"	iretq\n"		\
	".previous\n")

#endif // UROS_STUB_H
