/*
 *文件名: pmm_test.c.
 *作者: 李程鹏 on 18-2-20.
 *描述: 对物理内存管理模块的测试
*/

#include "tests.h"
#include "pmm.h"

// 物理内存页的数量
extern uint32_t phy_page_count;

// 对物理内存管理模块的测试
void pmm_test(){
    show_kernel_address();
    show_pmm_address();
    init_pmm();
    int i=0;
    print_color(rc_green,rc_black,"There  are total allocate #d pages\n",phy_page_count);
    for(;i<10;i++){
    	uint32_t pageAddress=pmm_alloc_page();
    	print("\nAllocate Address: #x",pageAddress);    	
    }
}