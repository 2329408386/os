// console_test.c

#include "types.h"
#include "tests.h"
#include "console.h"

static void test_clear()
{
	console_clear();
}

static void test_putc_color()
{
	char str[100]="abcdefg";
	int i;
	for (i = 0; str[i]!='\0'; ++i)
	{
		console_putc_color(str[i],15,0);
	}

	console_putc_color('\b',15,0);
	console_putc_color('\t',15,0);
	console_putc_color('j',15,0);
	console_putc_color('\n',15,0);
	console_putc_color('r',15,0);
	console_putc_color('\r',15,0);

	for(i=0;i<24*80+10;i++)
		console_putc_color('r',15,0);
}

static void test_console_write()
{
	console_write("1 2 3 4 5 6 7 8 9 10\na b c d e f\t cc\b\nabc\r");
}

static void test_console_write_color()
{
	console_write_color("1 2 3 4 5 6 7 8 9 10\na b c d e f\t cc\b\nabc\n",4,0);
	console_write_color("\n1 2 3 4 5 6 7 8 9 10\na b c d e f\t cc\b\nabc\n",2,0);
	console_write_color("\n1 2 3 4 5 6 7 8 9 10\na b c d e f\t cc\b\nabc\n",1,0);
}

static void test_console_write_hex()
{
	console_write_hex(0xfafbfcfd,4,0);
	console_putc_color('\n',15,0);
	console_write_hex(10,4,0);
	console_putc_color('\n',15,0);
	console_write_hex(-1,4,0);
	console_putc_color('\n',15,0);
	console_write_hex(0,4,0);
}

static void test_console_write_dec()
{
	console_write_dec(0xfafbfcfd,4,0);
	console_putc_color('\n',15,0);
	console_write_dec(10,4,0);
	console_putc_color('\n',15,0);
	console_write_dec(-1,4,0);
	console_putc_color('\n',15,0);
	console_write_dec(0,4,0);
}

static void test_console_write_dec_sign()
{
	console_write_dec_sign(-1,rc_red,rc_black);
	console_putc_color('\n',rc_white,rc_black);
	console_write_dec_sign(0,rc_red,rc_black);
	console_putc_color('\n',rc_white,rc_black);
	console_write_dec_sign(-12345,rc_red,rc_black);
	console_putc_color('\n',rc_white,rc_black);
	console_write_dec_sign(0xfafbfcfd,rc_red,rc_green);
	console_putc_color('\n',rc_white,rc_black);
	console_write_dec_sign(7481481,rc_red,rc_green);
}

void console_test()
{
	test_clear();
	// test_putc_color();
	// test_console_write();
	// test_console_write_color();
	// test_console_write_hex();
	// test_console_write_dec();
	test_console_write_dec_sign();
}