/*
 * =====================================================================================
 *
 *       Filename:  entry.c
 *
 *    Description:  hurlex 内核的入口函数
 *
 *        Version:  1.0
 *        Created:  2013年10月31日 13时31分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Hurley (LiuHuan), liuhuan1992@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include "types.h"
#include "console.h"
#include "tests.h"

int kernel_entry()
{
	console_clear();
//	 console_test();
//	string_test();
//	print_test();
    debug_test();
	return 0;
}

