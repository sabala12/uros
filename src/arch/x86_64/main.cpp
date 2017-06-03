#if defined(__linux__)
//#error "__linux__ is defined, you are not using a cross-compiler."
#endif

#include <utils/fmt.h>
#include <drivers/vga.h>

#include <arch/x86_64/bootparam.h>

int main(setup_header* mbh)
{
    size_t a;
    //asm volatile ("pop %0" : "=r"(a));

    (void)mbh;
    int *d = (int*)0x333333;

    vga::init_buffer();

    fmt::print("mbh = %d", (size_t)mbh);
    fmt::print("mbh = %d", a);


    return 0;
}
