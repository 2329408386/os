/*
 *文件名: heap.c.
 *作者: 李程鹏 on 18-3-4. (有参见hurley的heap实现).
 *描述: 
*/

#include "heap.h"
#include "vmm.h"
#include "pmm.h"
#include "types.h"

// 当前堆所在的位置.
static uint32_t heap_max=HEAP_START;

// 管理内存块的头指针.
static header_t* heap_first;

// 分配从start开始的内存页.
static void alloc_chunk(uint32_t start,uint32_t len){
    while(start+len>heap_max){
        uint32_t page=pmm_alloc_page();
        map(pgd_kern, heap_max, page, PAGE_PRESENT | PAGE_WRITE);
        heap_max+=PAGE_SIZE;
    }
}

// 释放指定的块
static void free_chunk(header_t* chunk){
    // 把当前块置空
    if(chunk->prev==0){
        heap_first=0;
    }else{
        chunk->prev->next=0;
    }

    //空闲的内存超过一页就释放掉
    while((heap_max-PAGE_SIZE)>=(uint32_t)chunk){
        heap_max-=PAGE_SIZE;
        uint32_t page;
        get_mapping(pgd_kern, heap_max, &page);
        unmap(pgd_kern, heap_max);
        pmm_free_page(page);
    }
}

// 分解块, 使其适配需要的长度
static void split_chunk(header_t* chunk,uint32_t len){
    //切分的条件是使需要的块大小不小于
    if(chunk->length-len> sizeof(header_t)){
        header_t* newChunk=(header_t*)((uint32_t)chunk+chunk->length);

        newChunk->prev=chunk;
        newChunk->next=chunk->next;
        newChunk->allocated=0;
        newChunk->length=chunk->length-len;

        chunk->next=newChunk;
        chunk->length=len;
    }
}

// 链合相邻且未使用的块
static void glue_chunk(header_t* chunk){
    // 判断该内存块前后内存块是否存在且未被分配
    if(chunk->next&&chunk->next->allocated==0){
        chunk->length=chunk->length+chunk->next->length;
        if(chunk->next->next)
            chunk->next->next->prev=chunk;
        chunk->next=chunk->next->next;
    }
    if(chunk->prev&&chunk->prev->allocated==0){
        if(chunk->prev->prev)
            chunk->prev->prev->next=chunk;
        chunk->prev=chunk->prev->prev;
    }

    // 如果该内存块后面没有内存了,就直接释放掉该内存块
    if(chunk->next==0)
        free_chunk(chunk);
}

// 初始化内核堆.
void init_heap(){
    heap_first=0;
}

// 申请长度为len的内核空间
void *kmalloc(uint32_t len){
    // 所有申请的长度是需要的内存长度加上管理该部分内存需要的长度.
    len+= sizeof(header_t);

    // 指定当前指针和上一个指针.
    header_t* cur_header=heap_first;
    header_t* prev_header=0;

    // 这些是在空闲块中有满足条件的块时.
    while (cur_header){
        // 判断当前块的条件
        if(cur_header->allocated==0&&cur_header->length>=len){
            split_chunk(cur_header,len);
            cur_header->allocated=1;
            // 因为这些都是供用户使用的工具,所以现在可能还体会不到它的具体作用.
            return (void*)((uint32_t)cur_header+ sizeof(header_t));
        }else{
            // 继续向后找到满足条件的块
            prev_header=cur_header;
            cur_header=cur_header->next;
        }
    }

    // 当没有满足的空闲块时,就需要申请新的内存空间(去找内存页来分配了).

    uint32_t chunk_start;   //申请内存块的起始位置.

    // 第一次初始化的时候,把起始位置设置为堆开始处的位置, 以后就是把上一次分配块的结束位置加上它的长度
    if(prev_header){
        chunk_start=(uint32_t)prev_header+prev_header->length;
    }else{
        chunk_start=HEAP_START;
        heap_first=(header_t*)chunk_start;
    }

    // 分配满足条件的块.
    alloc_chunk(chunk_start, len);
    cur_header=(header_t*)chunk_start;
    cur_header->prev=prev_header;
    cur_header->next=0;
    cur_header->allocated=1;
    cur_header->length=len;

    if(prev_header)
        prev_header->next=cur_header;

    return (void*)(chunk_start+ sizeof(header_t));
}

// 内存释放
void kfree(void* p){
    // 把当前块s设置为未分配状态
    header_t* header=(header_t*)((uint32_t)p- sizeof(header_t));
    header->allocated=0;

    // 链合相邻的空闲块
    glue_chunk(header);
}