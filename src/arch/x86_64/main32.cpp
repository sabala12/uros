//#include <drivers/vga.h>
//#include <stddef.h>

#include <stdint.h>
#include <arch/x86_64/bootparam.h>

/*static uint16_t* m_buffer = (uint16_t*)0xB8000;

static uint8_t m_color;
static constexpr size_t m_width = 80;
static constexpr size_t m_height = 25;

enum Color {
    Black = 0,
    Blue = 1,
    Green = 2,
};

static inline void set_color(enum Color fg, enum Color bg)
{
    m_color = fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

static inline void write_char(size_t row, size_t col, char c)
{
    const size_t index = (row * m_width) + col;
    m_buffer[index] = vga_entry(c, m_color);
}*/

//setup_header* mbh
extern "C" int main32(setup_header* mbh)
{
    //set_color(Color::Green, Color::Black);
    //write_char(0, 0, 'd');
    //(void)mbh;
    //vga::init();

    //vga::set_color(vga::Color::Green, vga::Color::Black);

    //int *d = (int*)0x333333;

    setup_header* setup_header_mem = (setup_header*)0x0800;
    *setup_header_mem = *mbh;

    return 0;
}