/**
*对控制台基本操作的实现
*/

#include "console.h"
#include "common.h"
#include "types.h"
#include "vmm.h"

// VGA(vedio graphics array)的起始地址
static  uint16_t* vedio_memory=(uint16_t*)(0xb8000+PAGE_OFFSET);

// 光标的x,y坐标
static uint8_t x_cursor=0;
static uint8_t y_cursor=0;

// 这个符号表示空格
static uint16_t blank=0x0f20;		

static char num_maps[]="0123456789abcdef";

// 把光标所在的位置信息传递给硬件
static void move_cursor()
{
	uint16_t cursorLocation=y_cursor*80+x_cursor;

	// 把光标信息传递给接口,14表示传递高8位,15表示传递低8位
	outb(0x3d4,14);
	outb(0x3d5,cursorLocation>>8);
	outb(0x3d4,15);
	outb(0x3d5,cursorLocation);
}

// 滚动屏幕
static void scroll()
{
	uint8_t i,j;
	uint16_t cursorLocation;

	if(y_cursor>=25)
	{
		// 把上一行替换成当前行
		for(i=0;i<24;i++)
		{
			for(j=0;j<80;j++)
			{
				cursorLocation=i*80+j;
				vedio_memory[cursorLocation]=vedio_memory[cursorLocation+80];
			}
		}

		// 把最后一行清空(其实就是用空格替换).
		cursorLocation=24*80;
		for(j=0;j<80;j++)
			vedio_memory[cursorLocation+j]=blank;

		// 将光标上移一行..
		y_cursor--;
	}
}

// 清屏操作
void console_clear()
{
	uint16_t end=24*80;
	uint16_t i;
	for(i=0;i<end;i++)
		vedio_memory[i]=blank;
	x_cursor=0;
	y_cursor=0;
	move_cursor();
}

// 输出一个带颜色的字符
void console_putc_color(char c,real_color_t fore,real_color_t back)
{
	// 获取指定颜色字符
	uint8_t backColor=(uint8_t)back;
	uint8_t foreColor=(uint8_t)fore;
	uint8_t color=(backColor<<4)|foreColor;
	uint16_t element=(color<<8)|c;

	//处理特殊字符(应该还有没考虑到的,常用的就这几个吧)
	// '\b'=退格,'\n'=换行,'\r'=回车,'\t'=Tab
	if (c=='\b'){
		x_cursor--;
		vedio_memory[y_cursor*80+x_cursor]=blank;		
	}
	else if(c=='\n'){
		x_cursor=0;
		y_cursor++;
	}
	else if(c=='\r'){x_cursor=0;}
	else if(c=='\t'){x_cursor+=4;}
	else if(c>=' '){	//根据ascii表,也只有大于等于32的ascii码对应的字符才有意思.
		vedio_memory[y_cursor*80+x_cursor]=element;
		x_cursor++;
	}

	// 判断光标越界的情况
	if(x_cursor<0){x_cursor+=80;y_cursor--;}
	if(x_cursor>=80){x_cursor-=80;y_cursor++;}

	scroll();		//有可能会用到滚动操作.
	move_cursor();		//移动光标的位置.
}

// 输出一个以'\0'结尾,黑色背景,白色前景色的字符串
void console_write(char *cstr)
{
	int i=0;
	for(;cstr[i]!='\0';i++)
	{
		console_putc_color(cstr[i],rc_white,rc_black);
	}
}

// 输出一个以'\0'结尾,自定义前景色和背景色的字符串
void console_write_color(char *cstr,real_color_t fore,real_color_t back)
{
	int i=0;
	for(;cstr[i]!='\0';i++)
	{
		console_putc_color(cstr[i],fore,back);
	}
}

// 以16进制的形式输出一个自定义前景色和背景色的无符号整数,假设该整数是符合条件的整数
void console_write_hex(uint32_t n,real_color_t fore,real_color_t back)
{
	uint8_t* position=(uint8_t*)(&n);
	char result[9];
	int i,resultPos=0;
	for(i=3;i>=0;i--)
	{
		result[resultPos]=num_maps[position[i]>>4];
		result[resultPos+1]=num_maps[ position[i]&0x0f];
		resultPos+=2;		
	}
	resultPos=0;
	while(resultPos<7&&result[resultPos]=='0')
		resultPos++;
	while(resultPos<8)
		console_putc_color(result[resultPos++],fore,back);
}

// 以10进制形式输出一个自定义前景色和背景色的无符号整数,假设该整数是符合条件的整数
void console_write_dec(uint32_t n,real_color_t fore,real_color_t back)
{
	char result[13];
	int pos=0;
	if(n==0){result[pos++]='0';}

	while(n>0){
		result[pos++]=n%10+'0';
		n/=10;
	}
	while(--pos>=0)
		console_putc_color(result[pos],fore,back);
}

// 以10进制形式输出一个自定义前景色和背景色的有符号整数,假设该整数是符合条件的整数
void console_write_dec_sign(int32_t n,real_color_t fore,real_color_t back)
{
	if(n<0)
	{
		console_putc_color('-',fore,back);
		n=-n;
	}
	console_write_dec(n,fore,back);
}

// 打印一个浮点数f,小数点后保留len位.
void console_write_float(float f,int len,real_color_t fore,real_color_t back)
{
    int i;
    int i_part;

//    判断正负
	if(f<0){
		console_putc_color('-',fore,back);
		f=-f;
	}

    // 打印整数部分
    i_part=(int)f;
    f=(f-i_part)*10;

    console_write_dec_sign(i_part,fore,back);
    console_putc_color('.',fore,back);

    // 获取len位长度的小数部分(太天真了,这里f还是需要一位一位的输出,不然超过范围了怎么办)
    for(i=0;i<len-1;i++){
        i_part=(int)f;
        f=(f-i_part)*10;
        console_write_dec_sign(i_part,fore,back);
    }

//    判断len+1位小数是否大于等于0.5来决定是否进位
    i_part=(int)f;
    if((f-i_part)>=0.5&&i_part<9)		// 这里为了节省内核栈,我就这样简化了,不定义局部变量数组来存放各个元素了, 对于精度要求很高的用户,抱歉啦!!
        i_part++;
    console_write_dec_sign(i_part,fore,back);
}