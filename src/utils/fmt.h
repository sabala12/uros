#ifndef	UROS_FMT_H
#define UROS_FMT_H

#include <drivers/vga.h>
#include <utils/print_type.h>

namespace fmt
{
	template <typename T>
	inline size_t handle_format_sign(const char* fmt, T arg)
	{
        /* Handle decimal sign */
        if (fmt[0] == 'd') {
            print_dec(arg);
			return 1;
		}

        if (fmt[0] == 'x') {
            print_hex(arg);
            return 1;
        }

        /* If no valid sign was found after %, simply print %. */
        if (fmt[0])
            vga_write("%");

		return 0;
	}

    /* Count number of characters until % sign */
	struct StopAtPercentSign
	{
        StopAtPercentSign()
			: m_count(0) { }

		inline bool operator()(const char* str, size_t i)
		{
			m_count++;
			return str[i] != '%';
		}

		size_t m_count;
	};

    static inline void print(const char* fmt)
    {
		vga_write(fmt);
		vga_write("\n");
    }

    template <typename A, typename... N>
    static inline void print(const char* fmt, A arg_a, N... arg_n)
    {
        /* Print until you reach the first % sign. */
	StopAtPercentSign condition;
        vga_write(fmt, condition);

        /* Exit if there are no more characters. */
        if (!condition.m_count)
            return;

        /* Handle a case were % is the last character */
        if (fmt[condition.m_count - 1] == '%' &&
            !fmt[condition.m_count])
            vga_write("%");

        /* Print the first argument by the format sign. */
        size_t arg_len = handle_format_sign(fmt + condition.m_count, arg_a);

        if (arg_len) {
            /* Go to next arg */
            print(fmt + condition.m_count + arg_len, arg_n...);
        } else {
            /* If the % sign didn't describe argument, repeat arg_a. */
            print(fmt + condition.m_count + arg_len, arg_a, arg_n...);
        }
    }
}

#endif // UROS_FMT_H
