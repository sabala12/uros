#if defined(__linux__)
//#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>
#include <arch/x86_64/bootparam.h>
#include <arch/x86_64/idt.h>

extern "C" int main64(setup_header* mbh)
{
    vga::init();

    fmt::print("Address Space: 0x0 -> 0xFFFFFFF");

    idt::setup_idt();


    //TODO::complete gdt


    return 0;
}
