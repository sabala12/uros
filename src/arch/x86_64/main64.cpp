#if defined(__linux__)
#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>
#include <arch/x86_64/bootparam.h>
#include <arch/x86_64/idt.h>

extern u64 p4_table;

extern "C" int main64(setup_header* mbh)
{
	fmt::print("uros kernel boot");
	fmt::print("ram_size=0x%x", mbh->ram_size);

	vga_init();

	idt_init();

	return 0;
}
