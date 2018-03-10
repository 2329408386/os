/**
*包含对各个模块的测试代码
*/

#ifndef INCLUDE_TESTS_H
#define INCLUDE_TESTS_H 

// 对console模块的测试接口
void console_test();

// 对string模块的测试接口
void string_test();

// 对于内核打印函数的测试
void print_test();

// 对debug模块的测试
void debug_test();

// 对gdt模块的测试.
void gdt_test();

// 对idt模块的测试
void idt_test();

// 对timer模块的测试
void timer_test();

// 对keyboard模块的测试
void keyboard_test();

// 对物理内存管理模块的测试
void pmm_test();

// 对内核堆的测试
void heap_test();

// 对内核线程切换的测试
void sched_test();

// 对用户操作的测试
void user_test();

#endif