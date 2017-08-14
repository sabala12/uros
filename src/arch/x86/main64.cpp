#if defined(__linux__)
//#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>
#include <arch/x86/bootparam.h>

extern uint64_t p4_table[];
extern uint64_t p3_table[];
extern uint64_t p2_table[];

extern "C" int main64(setup_header* mbh)
{
    vga::init();

    //TODO::init idt

    //TODO::complete gdt
    fmt::print("p3_table = 0x%d", (uint64_t)p4_table);
    //fmt::print("p3_table = 0x%d", (AA)p3_table);
    //fmt::print("p3_table = 0x%d", (AA)p2_table);

    return 0;
}
