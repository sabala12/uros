#ifndef	UROS_FMT_H
#define UROS_FMT_H

#include <drivers/vga.h>
#include <utils/print_type.h>

#define fmt_continue 1;
#define fmt_stop     0;

#define fmt_valid_sign   1;
#define fmt_invalid_sign 0;

template <typename T>
bool fmt_handle_format_sign(const char* fmt, T arg)
{
	switch (fmt[0]) {
		case 'd': {
			print_dec(arg);
			return fmt_valid_sign;
		}
		case 'x': {
			print_hex(arg);
			return fmt_valid_sign;
		}
		case 's': {
			print_str(arg);
			return fmt_valid_sign;
		}
	}

	return fmt_invalid_sign;
}

struct StopAtPercentSign
{
	StopAtPercentSign()
		: m_count(0), m_found(false) { }

	/* Count number of characters until % sign */
	/* Check if % sign is at the end of string. */
	bool operator()(const char* str, size_t i)
	{
		m_count++;
		if (str[i] == '%') {
			/* A valid sign cannot be at the end of a string. */
			if (str[i + 1] == '\0') {
				return fmt_continue;
			} else {
				m_found = true;
				return fmt_stop;
			}
		}
		return fmt_continue;
	}

	size_t m_count;
	bool   m_found;
};

static __attribute__ ((noinline)) void print(const char* fmt)
{
	vga_write_str(fmt);
	vga_write_str("\n");
}

template <typename A, typename... N>
static __attribute__ ((noinline)) void print(const char* fmt, A arg_a, N... arg_n)
{
	/* Print until you reach the first % sign. */
	StopAtPercentSign condition;
	vga_write_str(fmt, condition);

	/* Exit if there are no more characters. */
	if (!condition.m_found) {
		return;
	}

        /* Print the first argument by the format sign. */
	bool ret = fmt_handle_format_sign(fmt + condition.m_count, arg_a);

        if (ret) {
		print(fmt + condition.m_count + 1, arg_n...);
	} else {
		/* If the % sign didn't describe argument, repeat current arg. */
		print(fmt + condition.m_count, arg_a, arg_n...);
	}
}

#endif // UROS_FMT_H
