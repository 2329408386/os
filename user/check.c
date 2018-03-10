/*
*文件名: check.c
* 作者: 李程鹏 on Mar 10, 2018
* 描述: 对内部功能的检查
*/

#include "console.h"
#include "debug.h"
#include "pmm.h"
#include "heap.h"

// 物理内存页的数量
extern uint32_t phy_page_count;

// 对控制台模块的检查
void check_console(){
	print("\n The test for console is as follows:\n");
	
	print("char color: ");
	console_putc_color('j',15,0);
	print("\n");

	print("string: ");
	console_write("1 2 3 4 5 6 7 8 9 10\ta b c d e f\t cc\b\tabc\n");

	print("integer: ");
	console_write_dec_sign(-1,rc_red,rc_black);
	print("\n");

	print("unsigned integer: ");
	console_write_dec(-1,rc_red,rc_black);
	print("\n");

	print("float: ");
	console_write_float(-12.45,4,rc_red,rc_black);
	print("\n");
}

// 对内核堆的检查
void check_heap(){
	print_color(rc_red,rc_black,"The heap test results are as follow:\n");
	    void* addr1=kmalloc(100);
	    print("Address1:#x,length1=100bytes\n",addr1);
	    char* s=(char*)addr1;
	    s[0]='a';s[1]='b';s[2]='c';s[3]='\0';
	    print("#s\n",s);

	    void* addr2=kmalloc(500);
	    print("Address2:#x,length2=500bytes\n",addr2);
	    
	    kfree(addr1);
	    kfree(addr2);
}

// 对内存信息的检查
void check_memory(){
	show_kernel_address();
    	show_pmm_address();
    	print_color(rc_green,rc_black,"There  are total allocate #d pages\n",phy_page_count);
}