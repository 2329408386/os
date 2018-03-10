/*
 *文件名: pmm.c.
 *作者: 李程鹏 on 18-2-20.
 *描述: 对物理内存管理方法的一些实现
*/

#include "pmm.h"
#include "types.h"
#include "print.h"
#include "vmm.h"

// 存放物理内存栈
static uint32_t pmm_stack[PAGE_MAX_SIZE+1];

// 物理内存栈的栈顶
static uint32_t pmm_stack_top;

// 物理内存页的数量
uint32_t phy_page_count;

// 显示各段的物理地址及其可用性.
void show_pmm_address(){
    uint32_t address=glb_mboot_ptr->mmap_addr;      // 物理段的起始地址
    uint32_t len=glb_mboot_ptr->mmap_length/ sizeof(mmap_entry_t);        // 物理段的数量

    mmap_entry_t* entry=(mmap_entry_t*)(address+PAGE_OFFSET);
    int i=0;
    for(;i<len;i++){
        print("base: 0x#x\thigh: 0x#x\tsize: 0x#xbytes\ttype: 0x#x\n",entry[i].base_addr_low,entry[i].length_low+entry[i].base_addr_low,entry[i].length_low,entry[i].type);
    }
}

// 显示内核信息.
void show_kernel_address(){
    // 我已经不知道该说什么了,总之是很神奇的吧, 用一个变量来过度一些就好了!!
    uint32_t start=(uint32_t)&kern_start,end=(uint32_t)&kern_end;
    print("Kernel start address: 0x#x\t Kernel end address: 0x#x\tSize: #dkb\n",start,end,(end-start)/1024);
}

/*pmm_stack数组中的元素指向的地址就是一个一个的页空间.*/

// 初始化物理内存管理
void init_pmm(){
	uint32_t address=glb_mboot_ptr->mmap_addr;      // 物理段的起始地址
    	uint32_t len=glb_mboot_ptr->mmap_length/ sizeof(mmap_entry_t);        // 物理段的数量

    	// 获取物理段的起始地址.
    	mmap_entry_t* entry=(mmap_entry_t*)(address+PAGE_OFFSET);
    	int i=0;
    	for(;i<len;i++){
    		// 找到可用段的起始地址.
    		if(entry[i].type==1&&entry[i].base_addr_low==0x100000){    		
    			// 这里真是服了,为什么非要这样转换一下就可以了, 但是我又是怎么试出来的!!
    			uint32_t start=(uint32_t)&kern_start,end=(uint32_t)&kern_end;
    			// print("0x#x\n",end-start);
    			uint32_t page_addr=entry[i].base_addr_low+(end-start);	// 获取可分配页的起始地址.
    			// print("0x#x\n",page_addr);
    			uint32_t page_high_addr=entry[i].base_addr_low+entry[i].length_low;			//  获取可分配页的高位地址

    			// 为每个页分配对应的可用地址.
    			while(page_addr<page_high_addr&&page_addr<PMM_MAX_SIZE){
    				pmm_free_page(page_addr);
    				page_addr+=PMM_PAGE_SIZE;
    				phy_page_count++;
    			}
    		}
    	}
}

// 分配一个内存页的物理地址
uint32_t pmm_alloc_page(){
	if(pmm_stack_top<0){
		print_color(rc_red,rc_black,"There isn't any free space!");
	}
	uint32_t page=pmm_stack[pmm_stack_top--];	
	return page;
}

// 释放一个申请的内存
void pmm_free_page(uint32_t page){
	// 判断是否到了物理内存栈的最大空间
	if(pmm_stack_top>=PAGE_MAX_SIZE) {
		print_color(rc_red,rc_black,"You have already freed all the space!\n");
		return ;
	}
	pmm_stack[++pmm_stack_top]=page;	// 把当前栈指向的地址空间释放
}

// 页错误处理函数
void page_fault(regs* regs){

    // 当发生页错误时,程序想要获得的地址将会存放到CR2寄存器中.
    uint32_t cr2;
    asm volatile ("mov %%cr2, %0" : "=r" (cr2));

    print("Page fault at 0x#x, virtual faulting address 0x#x\n", regs->eip, cr2);
    print("Error code: #x\n", regs->error_code);

    // bit 0 为 0 指页面不存在内存里
    if ( !(regs->error_code & 0x1)) {
        print_color(rc_red,rc_black, "Because the page wasn't present.\n");
    }
    // bit 1 为 0 表示读错误，为 1 为写错误
    if (regs->error_code & 0x2) {
        print_color(rc_red,rc_black, "Write error.\n");
    } else {
        print_color(rc_red,rc_black, "Read error.\n");
    }
    // bit 2 为 1 表示在用户模式打断的，为 0 是在内核模式打断的
    if (regs->error_code & 0x4) {
        print_color(rc_red,rc_black, "In user mode.\n");
    } else {
        print_color(rc_red,rc_black, "In kernel mode.\n");
    }
    // bit 3 为 1 表示错误是由保留位覆盖造成的
    if (regs->error_code & 0x8) {
        print_color(rc_red,rc_black, "Reserved bits being overwritten.\n");
    }
    // bit 4 为 1 表示错误发生在取指令的时候
    if (regs->error_code & 0x10) {
        print_color(rc_red,rc_black, "The fault occurred during an instruction fetch.\n");
    }

    while (1);
}