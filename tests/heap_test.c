/*
 *文件名: heap_test.c.
 *作者: 李程鹏 on 18-3-5.
 *描述: 对内核堆的测试
*/

#include "tests.h"
#include "heap.h"
#include "print.h"
#include "types.h"

// 对内核堆的测试
void heap_test(){
    init_heap();

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