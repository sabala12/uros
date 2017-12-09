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

void set_color(enum vga_color fg, enum vga_color bg);

u16 vga_entry(unsigned char uc, u8 color);
char read_char(size_t row, size_t col);
void write_char(size_t row, size_t col, char c);
void clear_row(size_t row);
void shift_line();
void new_line();
bool is_new_line(const char* str, size_t i);
void check_shift();
void put_char(const char c);

struct RetTrue
{
	bool operator()(const char* str, size_t i)
	{
		(void)str;
		(void)i;
		return true;
	}
};

template <typename Condition>
static void write_str(const char* str, Condition& condition)
{
	size_t i = 0;
	while(str[i] && condition(str, i))
	{
		if (is_new_line(str, i)) {
			new_line();
		} else {
			put_char(str[i]);
		}
		i++;
	}
}

template <class Condition>
static void vga_write(const char* str, Condition& condition)
{
	write_str(str, condition);
}

static void vga_write(const char* str)
{
	RetTrue retTrue;
	write_str(str, retTrue);
}

void vga_init();

#endif // UROS_VGA
