#if defined(__linux__)
//#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>
#include <arch/x86/bootparam.h>

typedef uint64_t PT;

extern PT p4_table[];
extern PT p3_table[];
extern PT p2_table[];

extern uint32_t set_up_page_tables;
extern uint32_t enable_paging;

extern "C" int main64(setup_header* mbh)
{
    vga::init();

    //TODO::init idt

    //TODO::complete gdt
    fmt::print("set_up_page_tables = 0x%x", set_up_page_tables);
    fmt::print("p3_table = 0x%x", (PT)p4_table);

    return 0;


    fmt::print("p3_table = 0x%x", (PT)p3_table);
    fmt::print("p3_table = 0x%x", (PT)p2_table);

    return 0;
}
