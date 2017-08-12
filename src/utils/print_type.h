//
// Created by eran on 12/08/17.
//

#ifndef UROS_PRINT_TYPE_H
#define UROS_PRINT_TYPE_H

#include <utils/types.h>

namespace fmt
{
    template<typename T>
    inline void print_dec(T val);
    template<typename T>
    inline void print_hex(const T* val);
    template<typename T>
    inline void print_str(const T& val);

    template<typename T>
    inline void print_dec(T val)
    {
        const char *map[10] = {
                "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
        };

        /* Reverse negative values */
        if (val < 0) {
            vga::write("-");
            val *= -1;
        }

        if (val == 0) {
            vga::write(map[val]);
            return;
        }

        int len = 0;
        const int max_length = 9;
        uint8_t str[max_length];

        while (val != 0) {
            uint8_t v = val % 10;
            str[len] = v;
            len++;
            val /= 10;
        }

        for (; len > 0; len--) {
            vga::write(map[str[len-1]]);
        }
    }

    inline void print_hex_c(uint8_t val)
    {
        const char map[] = "0123456789abcdef";
        char hex[] = "0";
        hex[0] = map[val & 0xF];
        vga::write(hex);
    }

    template <typename T>
    inline void print_hex(T val)
    {
        uint64_t _val = (uint64_t)val;
        if (_val == 0) {
            vga::write("0");
            return;
        }

        int len = 0;
        const int max_length = 16;
        uint8_t str[max_length];

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
    inline void print_str(const T& val)
    {
        (void)val;
    }

    template <typename T>
    inline void print_dec(T* val)
    {
        (void)val;
    }
}

#endif //UROS_PRINT_TYPE_H
