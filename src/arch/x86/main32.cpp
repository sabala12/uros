#include <stdint.h>
#include <arch/x86/bootparam.h>

extern "C" int main32(setup_header* mbh)
{
    setup_header* setup_header_mem = (setup_header*)0x0800;
    *setup_header_mem = *mbh;

    return 0;
}