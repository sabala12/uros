#include <drivers/vga.h>

static u16* m_buffer = (u16*)0xB8000;
static constexpr size_t m_width = 80;
static constexpr size_t m_height = 25;
static constexpr char m_blank = ' ';	
static constexpr size_t last_line = m_width - 1;
	
static u8 m_color;
static u32 m_row;
static u32 m_col;

void set_color(enum vga_color fg, enum vga_color bg)
{
	m_color = fg | bg << 4;
}

u16 vga_entry(unsigned char uc, u8 color)
{
	return (u16) uc | (u16) color << 8;
}

char read_char(size_t row, size_t col)
{
	const size_t index = (row * m_width) + col;
	return m_buffer[index];
}

void write_char(size_t row, size_t col, char c)
{
	const size_t index = (row * m_width) + col;
	m_buffer[index] = vga_entry(c, m_color);
}

void clear_row(size_t row)
{
	for (size_t col = 0; col < m_width; col++)
		write_char(row, col, m_blank);
}

void shift_line()
{
	for (size_t row = 1; row < m_height; row++)
	{
		for (size_t col = 0; col < m_width; col++)
		{
			char c = read_char(row, col);
			write_char(row - 1, col, c);
		}
	}

	clear_row(last_line);
	m_col = 0;
}

void new_line()
{
	m_row++;
	m_col = 0;
}

bool is_new_line(const char* str, size_t i)
{
	return str[i] == '\n';
}

void check_shift()
{
	if (m_col == m_width)
	{
		if (m_row == last_line) {
			shift_line();
		} else {
			new_line();
		}
	}
}

void put_char(const char c)
{
	check_shift();
	write_char(m_row, m_col, c);
	m_col++;
}

void clear_screen()
{
	for (size_t row = 0; row < m_height; row++)
		clear_row(row);

	m_row = 0;
	m_col = 0;
}

void vga_init()
{
	clear_screen();
	set_color(Green, Black);
}
