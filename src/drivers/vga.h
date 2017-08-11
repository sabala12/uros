#ifndef UROS_KERNEL
#define UROS_KERNEL

#include <stddef.h>
#include <stdint.h>


namespace vga
{
		static uint16_t* m_buffer = (uint16_t*)0xB8000;
		static constexpr size_t m_width = 80;
		static constexpr size_t m_height = 25;
		static constexpr char m_blank = ' ';	
		static constexpr size_t last_line = m_width - 1;

		enum Color 
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

    	static uint8_t m_color;
    	static size_t m_row;
    	static size_t m_col;

    	size_t strlen(const char* str) 
    	{
    	    size_t len = 0;
    	    while (str[len])
    	        len++;

    	    return len;
    	}

    	static inline void set_color(enum Color fg, enum Color bg)
    	{
    	    m_color = fg | bg << 4;
    	}

    	static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
    	{
    	    return (uint16_t) uc | (uint16_t) color << 8;
    	}

    	static inline char read_char(size_t row, size_t col)
    	{
    	    const size_t index = (row * m_width) + col;
    	    return m_buffer[index];
    	}

    	static inline void write_char(size_t row, size_t col, char c)
    	{
    	    const size_t index = (row * m_width) + col;
    	    m_buffer[index] = vga_entry(c, m_color);
    	}

    	static void clear_row(size_t row)
    	{
    	    for (size_t col = 0; col < m_width; col++)
    	        write_char(row, col, m_blank);
    	}

    	static void shift_line()
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
    	
    	static inline void new_line()
    	{
    	    m_row++;
    	    m_col = 0;
    	}

		static inline bool is_new_line(const char* str, size_t i)
		{
			return str[i] == '\n';
		}

		static inline void check_shift()
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
					check_shift();
					write_char(m_row, m_col, str[i]);
					m_col++;
				}
				i++;
			}
		}

		template <class Condition>
		inline void write(const char* str, Condition& condition)
		{
			write_str(str, condition);
		}

		inline void write(const char* str)
		{
			RetTrue retTrue;
			write_str(str, retTrue);
		}

		template <class Condition>
		inline void writeln(const char* str, Condition& condition)
		{
			write_str(str, condition);
            new_line();
		}

		inline void writeln(const char* str)
    	{
			RetTrue retTrue;
			write_str(str, retTrue);
            new_line();
    	}

    	static void clear_screen()
    	{
    	    for (size_t row = 0; row < m_height; row++)
    	        clear_row(row);

    	    m_row = 0;
    	    m_col = 0;
    	}

    	static void init()
    	{
    	    clear_screen();
			vga::set_color(vga::Green, vga::Black);
    	}
}

#endif // UROS_KERNEL
