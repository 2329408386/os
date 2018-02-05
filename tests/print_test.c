/*
 *文件名: print_test.c.
 *作者: 李程鹏 on 18-2-3.
 *描述:  对于内核打印函数的测试
*/

#include "print.h"
#include "console.h"

// 对print_color()函数的测试.
void print_color_test()
{
    int i=0;
    int a=-10;
    char str[10]="1234789";
    int b=20;
    float f=-1.235698;
    double d=1.459;

    // 然后我这样就愉快的改变了a的值,哈哈,至少给个提醒吧,哥哥!!!
    str[10]=0xff;        //我的天,快要崩溃了,c居然不报错啊啊啊啊!!!!!
    str[11]=0xff;
    str[12]=0xff;
    str[13]=0xff;


    //测试没有标志符号(就是#符号)时
    print_color(rc_white,rc_black,"abde8786\n");

    //测试打印多少空格
    print_color(rc_white,rc_black,"abde#8ac#7ui86\n");

    //测试#后面啥都没有时
    print_color(rc_white,rc_black,"abde#8ac#7ui86#");

    // 测试#后面跟了一些空格类字符时
    print_color(rc_white,rc_black,"\nabde# ac#\tui86#\n");

    // 测试#后面跟不合规定的字符时
    print_color(rc_white,rc_black,"\nabde# ac#\tui86#\n#yy");

    // 测试整数
    print_color(rc_white,rc_black,"\n#d#4faj#d#2#d\n",a,-a,500);
    print_color(rc_white,rc_black,"\n#d#4faj\n",-a);
    print_color(rc_white,rc_black,"\n#d#4faj\n",100);

    // 测试16进制整数
    print_color(rc_white,rc_black,"\n#x#4faj#x#2#x###x\n",a,-a,500,0x1234);

    // 测试无符号整数
    print_color(rc_white,rc_black,"int 32:#d\tunsigned int 32:#u\thex:#x\n",a,a,a);

    // 测试字符
    print_color(rc_white,rc_black,"#4#c#4#c\n",97,99);

    // 测试字符串
    print_color(rc_white,rc_black,"#s\n#x\t#x\t#x\n",str,&a,&str,&b);

    // 测试指针
    for(;str[i]!=0;i++)
        print_color(rc_white,rc_black,"#x#2",&str[i]);
    print_color(rc_white,rc_black,"\n");
    print_color(rc_white,rc_black,"a=#x\n",a);

    // 测试浮点数
    print_color(rc_white,rc_black,"#f\t#f2\t#f4\t#f\n",f,-f,1231.3131231,23.9);
    print_color(rc_white,rc_black,"#f3\n",f);

    print_color(rc_white,rc_black,"#l70",d);
}

// 对于内核打印函数的测试
void print_test(){
//    print_color_test();

    float f=-1.235698;
    print("#f\t#f2\t#f4\t#f\n",f,-f,1231.3131231,23.9);
}