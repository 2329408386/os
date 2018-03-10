/*
*文件名: init.c
* 作者: 李程鹏 on Mar, 2018
*/

#include "user.h"
#include "init.h"
#include "multiboot.h"
#include "types.h"
#include "console.h"
#include "tests.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "keyboard.h"
#include "vmm.h"
#include "pmm.h"
#include "sched.h"
#include "task.h"
#include "heap.h"

// 内核栈的栈顶
uint32_t kern_stack_top;

// 内核初始化函数
void kern_init();

// 开启分页机制之后的 Multiboot 数据指针
multiboot_t *glb_mboot_ptr;

// 开启分页机制之前的Multiboot指针, 在boot.s中申明
extern multiboot_t *mboot_ptr_tmp;

// 开启分页机制之后的内核栈
char kern_stack[STACK_SIZE];

// 内核使用的临时页表和页目录
// 该地址必须是页对齐的地址，使用1MB以下地址空间中的12KB来暂时放置临时页表
__attribute__((section(".init.data"))) pgd_t *pgd_tmp  = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low  = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

// 内核入口函数
__attribute__((section(".init.text"))) void kernel_entry()
{
	pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;

    // 映射高位页目录.
	pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;

	// 映射内核虚拟地址 4MB 到物理地址的前 4MB
	int i;
	for (i = 0; i < 1024; i++) {
		pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

    // 好吧, 假设懂了, 这里就了解大概吧, 知道是怎么回事就好了.
	// 映射 0x00000000-0x00400000 的物理地址到虚拟地址 0xC0000000-0xC0400000
	for (i = 0; i < 1024; i++) {
		pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	// 设置临时页表
	asm volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));

	uint32_t cr0;

	// 启用分页，将 cr0 寄存器的分页位置为 1 就好
	asm volatile ("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0" : : "r" (cr0));

	// 切换内核栈
	kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
	asm volatile ("mov %0, %%esp\n\t"
			"xor %%ebp, %%ebp" : : "r" (kern_stack_top));

	// 更新全局 multiboot_t 指针
	glb_mboot_ptr = mboot_ptr_tmp;
    glb_mboot_ptr=(uint32_t)glb_mboot_ptr+0xc0000000;

	// 调用内核初始化函数(从这以后, 内核地址就属于3G到4G之间了, 然后通过TLB转换成对应的物理地址.
	kern_init();
}

void init(){
	console_clear();

  	init_debug();
   	init_gdt();
   	init_idt();
    	init_vmm();
    	init_timer(200);
    	init_pmm();
    	init_heap();
	init_sched();
	init_user();
}