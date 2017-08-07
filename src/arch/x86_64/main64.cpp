#if defined(__linux__)
//#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>
#include <arch/x86_64/bootparam.h>

extern "C" int main64(setup_header* mbh)
{
    vga::init();

    //setup_header* mbh = (setup_header*)0x0800;

    fmt::print("mbh->setup_sects = %d", mbh->setup_sects);
    fmt::print("mbh->root_flags = %d", mbh->root_flags);


    //fmt::print("mbh = %d", (size_t)mbh);
    //fmt::print("mbh = %d", a);

    return 0;
}
