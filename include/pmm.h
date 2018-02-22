/*
 *文件名: pmm.h.
 *作者: 李程鹏 on 18-2-20.
 *描述: 对物理内存管理的一些定义.
*/

#ifndef OS_PMM_H
#define OS_PMM_H

#include "multiboot.h"
#include "print.h"
#include "types.h"

// 定义栈的大小
#define STACK_SIZE 8192

// qemu支持的最大物理内存为(512M)
#define PMM_MAX_SIZE 0x20000000

// 定义一个页大小为4kb
#define PMM_PAGE_SIZE 0x1000

// 最大支持的物理页面个数
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE)

// 页掩码按照4096bytes对齐
#define PHY_PAGE_MASK 0xfffff000

// 声明全局的 multiboot_t * 指针(最开始的定义在boot.s文件中).
extern multiboot_t *glb_mboot_ptr;

// 在kernel.ld文件中声明的内核开始位置和结束位置.
extern uint32_t kern_start;
extern uint32_t kern_end;

// 显示各段的物理地址及其可用性.
void show_pmm_address();

// 显示内核信息.
void show_kernel_address();

// 初始化物理内存管理
void init_pmm();

// 分配一个内存页的物理地址
uint32_t pmm_alloc_page();

// 释放一个申请的内存
void pmm_free_page(uint32_t page);

#endif //OS_PMM_H
