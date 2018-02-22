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
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"

int kernel_entry()
{	
	console_clear();

  	init_debug();
   	init_gdt();
   	init_idt();
//  init_timer(20);
//	init_keyboard();
//	asm("int $33");
//	keyboard_test();
	pmm_test();
	return 0;
}

