/*
 *文件名: keyboard.c.
 *作者: 李程鹏 on 18-2-16.
 *描述: 完成键盘处理函数
*/

#include "keyboard.h"
#include "types.h"
#include "print.h"
#include "common.h"
#include "idt.h"
#include "heap.h"

// 扫描码-ascii码对应表, 索引是扫描码, 值是ascii码
static unsigned char keymaps[128] =
        {
                0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
                '9', '0', '-', '=', '\b',	/* Backspace */
                '\t',			/* Tab */
                'q', 'w', 'e', 'r',	/* 19 */
                't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
                0,			/* 29   - Control */
                'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
                '\'', '`',   0,		/* Left shift */
                '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
                'm', ',', '.', '/',   0,				/* Right shift */
                '*',
                0,	/* Alt */
                ' ',	/* Space bar */
                0,	/* Caps lock */
                0,	/* 59 - F1 key ... > */
                0,   0,   0,   0,   0,   0,   0,   0,
                0,	/* < ... F10 */
                0,	/* 69 - Num lock*/
                0,	/* Scroll Lock */
                0,	/* Home key */
                0,	/* Up Arrow */
                0,	/* Page Up */
                '-',
                0,	/* Left Arrow */
                0,
                0,	/* Right Arrow */
                '+',
                0,	/* 79 - End key*/
                0,	/* Down Arrow */
                0,	/* Page Down */
                0,	/* Insert Key */
                0,	/* Delete Key */
                0,   0,   0,
                0,	/* F11 Key */
                0,	/* F12 Key */
                0,	/* All other keys are undefined */
        };

static uint8_t isCap=0;     //判断是否大写锁定
static char buff[1024];
static uint32_t pos=0;
static uint8_t is_finished=1;


// 键盘输入回调函数
void keyboard_call(regs* registers){
    // 控制键盘开启与关闭(0xad表示关闭键盘输入,0xae表示打开键盘输入)

    uint8_t state=0;        // 记录当前键盘缓冲器的状态, 如果为0,则说明键盘缓冲器中没有内容
    uint8_t scanCode=0;        // 如果返回数据为0, 表示读取数据失败了

    outb(0x64,0xad);    //关闭键盘输入
    state=inb(0x64);    //获取当前缓冲器的状态

    if(state==0)
        goto end;

    //当键盘缓冲区中有数据时,读取数据
    scanCode=inb(0x60);

    // 因为一个键有2个扫描码,按下时和断开时, 我只处理按下时的扫描码.
    if(scanCode>=0x80)
        goto end;

    if(keymaps[scanCode]=='\n')     //回车键代表输入的结束.
    {
        is_finished=0;
        buff[pos]=0;
        pos=0;
        goto end;
    }

    if(scanCode==0x3a)     // 大写锁定键
    {
        isCap=isCap^1;
        goto end;
    }

    // 当大写锁定的时候,保存一个按键上方对应的ascii值.
    if(isCap){
        if(keymaps[scanCode]>='a'&&keymaps[scanCode]<='z')
            buff[pos]=keymaps[scanCode]-32;
        else{
            switch (keymaps[scanCode]){
                case '`': buff[pos]='~';break;
                case '1': buff[pos]='!';break;
                case '2': buff[pos]='@';break;
                case '3': buff[pos]='#';break;
                case '4': buff[pos]='$';break;
                case '5': buff[pos]='%';break;
                case '6': buff[pos]='^';break;
                case '7': buff[pos]='&';break;
                case '8': buff[pos]='*';break;
                case '9': buff[pos]='(';break;
                case '0': buff[pos]=')';break;
                case '-': buff[pos]='_';break;
                case '=': buff[pos]='+';break;
                case '[': buff[pos]='{';break;
                case ']': buff[pos]='}';break;
                case '\\':buff[pos]='|';break;
                case ';': buff[pos]=':';break;
                case '\'':buff[pos]='"';break;
                case ',': buff[pos]='<';break;
                case '.': buff[pos]='>';break;
                case '/': buff[pos]='?';break;
                default: buff[pos]=keymaps[scanCode];
            }
        }
    }else{
        buff[pos]=keymaps[scanCode];      //该扫描码对应的ascii码.
    }
    print("#c",buff[pos]);
    pos++;

    end:outb(0x64,0xae);    //打开键盘中断
}



// 初始化键盘输入函数.
void init_keyboard(){
    register_interrupt_handler(33,keyboard_call);
    asm volatile("sti");    //打开中断
}

// 输入一个字符串
char* scanf(){
    char* toReturn=(char*)kmalloc(1024);        // 如果不想让它返回相同的地址空间,请重新为它分配内存, 不debug想都想不到这里会出错.
    init_keyboard();
    is_finished=1;
    while(is_finished)
        ;
    strcpy(toReturn,buff);
    return toReturn;
}