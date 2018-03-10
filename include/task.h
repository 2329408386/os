/*
 *文件名: task.h.
 *作者: 李程鹏 on 18-3-6.
 *描述: 定义关于任务创建与切换的数据结构和函数
*/

#ifndef OS_TASK_H
#define OS_TASK_H

#include "types.h"
#include "vmm.h"
#include "pmm.h"

typedef uint32_t pid_t;     //定义进程标志类型.

// 进程状态描述
typedef enum task_state{
    TASK_UNINIT=0,      //任务未初始化状态
    TASK_SLEEPING=1,    //任务沉睡状态
    TASK_RUNNING=2,     //任务可运行或正在运行状态
    TASK_ZOMBIE=3,      //僵尸态
}task_state;

// 内核上下文切换需要保存的信息
typedef struct context{
    uint32_t esp;
    uint32_t ebp;
    uint32_t ebx;
    uint32_t esi;
    uint32_t edi;
    uint32_t eflags;
}context;

// 进程内存地址结构
typedef struct mm_struct{
    pgd_t* pgd_dir;     //进程页表
}mm_struct;

// 进程控制块PCB
typedef struct pcb{
    task_state state;   //进程当前状态
    pid_t  pid;     //进程标识符
    void* stack;    //进程的内核栈地址(每个内存都为其分配单独的栈地址)
    mm_struct* mm;  //当前进程的内核地址映像
    context context;    //进程切换需要的上下文信息
    struct pcb* next;   //下一个PCB
}pcb;

// 全局pid(可以用map把pid和对应的pcb模块关联起来).
extern pid_t now_pid;

// 内核线程的创建
uint32_t kernel_thread(int (*fn)(void* ) ,void* arg);

// 线程退出函数(指的就是退出当前线程)
void kthread_exit();

#endif //OS_TASK_H
