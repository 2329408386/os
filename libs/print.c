/*
 *文件名: print.c.
 *作者: 李程鹏 on 18-2-3.
 *描述: 
*/

#include "print.h"
#include "vargs.h"
#include "console.h"
#include "types.h"

// 判断一个字符是否是数字范围
static int isDigit(char c){
    if(c>='0'&&c<='9') return 1;
    return 0;
}

// 将一个字符串转换成一个整数输出, 这里默认字符串代表的是一个自然数
static int to_number(char* str){
    int result=0,i=0;
    for (;str[i]!=0;i++)
        result=result*10+(str[i]-'0');
    return result;
}

// 打印指定长度的空格
static void print_blank(int len){
    int i=0;
    for(;i<len;i++)
        console_putc_color(' ',rc_white,rc_black);
}

// 内核级的屏幕打印函数,一般不提供外部调用
void printk_color(real_color_t fore,real_color_t back,char* format,va_list list)
{
    // 记录当前的输出位置
    int i=0;

    // 临时存放字符串
    char temp[10];int tempPos=0;

    for(;format[i]!=0;i++)
    {
        // 检测字符是否为井号, 如果不是,则说明是一般字符,直接打印出来就好了,要输出一个井号,则需要输入2个井号
        if(format[i]!='#'){
            console_putc_color(format[i],fore,back);
            continue;
        }

        i++;
        switch (format[i]){
            case 0:     //不小心到达字符串末尾了
                console_putc_color(format[i-1],fore,back);
                goto charEnd;
            case '#':   //对的,如果要输出#号,你需要打2个
                console_putc_color(format[i-1],fore,back);
                break;
            case 'd':   //整数
                console_write_dec_sign(va_arg(list,int32_t),fore,back);
                break;
            case 'x':   //十六进制整数
                console_write_hex(va_arg(list,uint32_t),fore,back);
                break;
            case 'u':   //无符号整数
                console_write_dec(va_arg(list,uint32_t),fore,back);
                break;
            case 'c':   //字符
                console_putc_color((uint8_t)(va_arg(list,uint32_t)),fore,back);
                break;
            case 's':   //字符串
                console_write_color(va_arg(list,char*),fore,back);
                break;
            case 'l':
            case 'f':   //浮点数
                // 'f'之后紧跟数字表示要保留的小数点个数
                i++;
                if(isDigit(format[i])){
                    temp[tempPos]=format[i];
                    while (isDigit(format[++i]))
                        temp[++tempPos]=format[i];
                    temp[++tempPos]=0;
                    // console_write_dec_sign(to_number(temp),fore,back);
                   console_write_float(va_arg(list,double),to_number(temp),fore,back);
                    tempPos=0;
                }else
                    console_write_float(va_arg(list,double),6,fore,back);
                i--;
                break;
            default:
                // '#'之后紧跟数字表示要输出多少个空格
                if(isDigit(format[i])){
                    temp[tempPos]=format[i];
                    while (isDigit(format[++i]))
                        temp[++tempPos]=format[i];
                    temp[++tempPos]=0;
                    print_blank(to_number(temp));       //这里temp可不能用指针啊,不小心就把输入的format给改变了.
                    tempPos=0;i--;
                }
                //  这么多规则都不匹配之后,只能说明用户的输入有问题,或者还有什么我没有想到的地方(这里纯属设计问题了哈)
                else{
                    if(format[i]==' '||format[i]=='\t'||format[i]=='\n'||format[i]=='\f'){
                        console_putc_color(format[i-1],fore,back);
                        console_putc_color(format[i],fore,back);
                    }
                    else{
                        console_write_color("No matching rules,please check your input!",rc_red,rc_black);
                        goto charEnd;       //切切, 老师叫不用就不用啊.
                    }

                }
        }
    }

    charEnd:;
}

// 根据给出的字符串的格式和可变参数列表以及颜色,打印带颜色的字符串
void print_color(real_color_t fore,real_color_t back,char* format,...){
    va_list list;
    va_start(list,format);
    printk_color(fore,back,format,list);
    va_end(list);
}

// 根据给出的字符串的格式和可变参数列表,打印白字黑底的字符串
void print(char* format,...){
    va_list list;
    va_start(list,format);
    printk_color(rc_white,rc_black,format,list);
    va_end(list);
}