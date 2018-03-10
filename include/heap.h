/*
 *文件名: heap.h.
 *作者: 李程鹏 on 18-3-4.
 *描述: 对于内存堆的管理
*/

#ifndef OS_HEAP_H
#define OS_HEAP_H

#include "types.h"

// 内核堆起始地址(因为内核页表从0xc0000000开始,并且占据512MB, 所以设置内核堆起始地址为这个).
#define HEAP_START 0xe0000000

// 管理内核堆的指针
typedef struct header{
    // 指向下一个和上一个内核堆
    struct header* prev;
    struct header* next;

    //! 用位域的方式定义该内核块的长度和是否被分配
    uint32_t allocated:1;
    uint32_t length:31;
}header_t;

// 初始化内核堆
void init_heap();

// 申请长度为len的内核空间
void *kmalloc(uint32_t len);

// 内存释放
void kfree(void* p);

#endif //OS_HEAP_H
