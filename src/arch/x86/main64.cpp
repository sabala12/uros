#if defined(__linux__)
//#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>
#include <arch/x86/bootparam.h>

extern "C" int main64(setup_header* mbh)
{
    vga::init();



    fmt::print("mbh->ram_size = 0x%d", mbh->ram_size);
    fmt::print("mbh->syssize = 0x%d", mbh->syssize);
    fmt::print("mbh->setup_sects = %d", mbh->setup_sects);
    fmt::print("mbh->root_flags = %d", mbh->root_flags);

    return 0;
}
