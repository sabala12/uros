#ifndef UROS_VGA_H
#define UROS_VGA_H

#include <libc/types.h>

enum vga_color 
{
	Black      = 0,
	Blue       = 1,
	Green      = 2,
	Cyan       = 3,
	Red        = 4,
	Magenta    = 5,
	Brown      = 6,
	LightGray  = 7,
	DarkGray   = 8,
	LightBlue  = 9,
	LightGreen = 10,
	LightCyan  = 11,
	LightRed   = 12,
	Pink       = 13,
	Yellow     = 14,
	White      = 15,
};

void vga_new_line();
void vga_put_char(const char c);
void vga_init();
void vga_write_str_handle_char(const char c);


template <typename Condition>
static void vga_write_str(const char* str, Condition& condition)
{
	size_t i = 0;
	while (str[i] && condition(str, i)) {
		vga_write_str_handle_char(str[i]);
		i++;
	}
}

static void vga_write_str(const char* str)
{
	size_t i = 0;
	while (str[i]) {
		vga_write_str_handle_char(str[i]);
		i++;
	}
}

#endif // UROS_VGA
