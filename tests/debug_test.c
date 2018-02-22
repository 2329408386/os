/*
 *文件名: debug_test.c.
 *作者: 李程鹏 on 18-2-5.
 *描述: 对debug模块的测试
*/

#include "tests.h"
#include "debug.h"

static void print_cur_status_test()
{
    print_cur_status();
    print_cur_status();
}

static void panic_test()
{
    panic("Panic test!");
}

// 对debug模块的测试
void debug_test(){
    init_debug();
    print_cur_status_test();
    panic_test();
}