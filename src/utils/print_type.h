//
// Created by eran on 12/08/17.
//

#ifndef UROS_PRINT_TYPE_H
#define UROS_PRINT_TYPE_H

#include <libc/types.h>

template<typename T>
inline void print_dec(T val);
template<typename T>
inline void print_hex(const T* val);
template<typename T>
inline void print_str(const T& val);

template<typename T>
void print_dec(T val)
    {
        const char *map[10] = {
                "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
        };

        /* Reverse negative values */
        if (val < 0) {
            vga_write_str("-");
            val *= -1;
        }

        if (val == 0) {
            vga_write_str(map[val]);
            return;
        }

        int len = 0;
        const int max_length = 9;
        u8 str[max_length];

        while (val != 0) {
            u8 v = val % 10;
            str[len] = v;
            len++;
            val /= 10;
        }

        for (; len > 0; len--) {
            vga_write_str(map[str[len-1]]);
        }
    }

    inline void print_hex_c(u8 val)
    {
        const char map[] = "0123456789abcdef";
        char hex[] = "0";
        hex[0] = map[val & 0xF];
        vga_write_str(hex);
    }

	template <typename T>
	void print_hex(T val)
	{
		u64 _val = (u64)val;
	    	if (_val == 0) {
			vga_write_str("0");
			return;
	    	}
	
	    	int len = 0;
	    	const int max_length = 16;
	    	u8 str[max_length];
	
	    	while (_val != 0) {
		    	str[len] = _val & 0xF;
		    	len++;
			_val = _val >> 4;
	    	}
	
	    	//TODO::ASSERT len <= 16
	    	for (; len > 0; len--) {
			print_hex_c(str[len-1]);
	    	}
	}
	
	template<typename T>
	void print_str(const T& val)
	{
	    	vga_write_str((const char*)val);
	}

	template<typename T>
	void print_str(const T* val)
	{
		vga_write_str((const char*)val);
	}

	template <typename T>
	void print_dec(T* val)
	{
		(void)val;
	}

#endif //UROS_PRINT_TYPE_H
