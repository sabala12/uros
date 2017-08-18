#include <stdint.h>
#include <arch/x86/memory.h>
#include <arch/x86/bootparam.h>



extern "C" int main32(setup_header* mbh)
{
    (void)mbh;
    memory::setup_page_table();
    return 0;
}