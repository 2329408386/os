; ----------------
; 作者: 李程鹏
; 时间: 2018年2月11日
; 描述: 对中断处理程序公共部分的定义(包括压入中断号,保存现场和恢复现场等)
;-----------------

;isr(interrupt service routine).定义没有中断代码的时的开始处理
%macro isr_no_error_code 1
[global isr%1]  ;%1表示第一个参数
isr%1:
    cli     ;关闭其它中断
    push 0  ;因为有的中断函数会自动压入错误码,有的不会,这里增加一个占位符,方便清栈
    push %1     ;压入中断号
    jmp isr_common_handler      ;跳转到中断处理的公共部分
%endmacro

;定义有中断代码的时的开始处理
%macro isr_error_code 1
[global isr%1]  ;%1表示第一个参数
isr%1:
    cli     ;关闭其它中断

    ;我也不知道为什么, 但是站在汇编级调试给了我灵感, 这里就是应该统一放入0嘛, 但愿我是错的, 毕竟是cpu保留
    ;这里就是出于学习的目的呗,不断的试错.
    ;push 0
    push %1     ;压入中断号
    jmp isr_common_handler      ;跳转到中断处理的公共部分
%endmacro

;定义中断请求函数的宏处理(PS:c的命名真的是一件不容易的事).
%macro irq_ 2
[global irq%1]
irq%1:
    cli
    push 0
    push %2
    jmp irq_common_handler
%endmacro

; 定义中断处理函数
isr_no_error_code  0 	; 0 #DE 除 0 异常
isr_no_error_code  1 	; 1 #DB 调试异常
isr_no_error_code  2 	; 2 NMI
isr_no_error_code  3 	; 3 BP 断点异常 
isr_no_error_code  4 	; 4 #OF 溢出 
isr_no_error_code  5 	; 5 #BR 对数组的引用超出边界 
isr_no_error_code  6 	; 6 #UD 无效或未定义的操作码 
isr_no_error_code  7 	; 7 #NM 设备不可用(无数学协处理器) 
isr_error_code    8 	; 8 #DF 双重故障(有错误代码) 
isr_no_error_code  9 	; 9 协处理器跨段操作
isr_error_code   10 	; 10 #TS 无效TSS(有错误代码) 
isr_error_code   11 	; 11 #NP 段不存在(有错误代码) 
isr_error_code   12 	; 12 #SS 栈错误(有错误代码) 
isr_error_code   13 	; 13 #GP 常规保护(有错误代码) 
isr_error_code   14 	; 14 #PF 页故障(有错误代码) 
isr_no_error_code 15 	; 15 CPU 保留 
isr_no_error_code 16 	; 16 #MF 浮点处理单元错误 
isr_error_code   17 	; 17 #AC 对齐检查 
isr_no_error_code 18 	; 18 #MC 机器检查 
isr_no_error_code 19 	; 19 #XM SIMD(单指令多数据)浮点异常

; 20~31 Intel 保留
isr_no_error_code 20
isr_no_error_code 21
isr_no_error_code 22
isr_no_error_code 23
isr_no_error_code 24
isr_no_error_code 25
isr_no_error_code 26
isr_no_error_code 27
isr_no_error_code 28
isr_no_error_code 29
isr_no_error_code 30
isr_no_error_code 31
; 32～255 用户自定义
isr_no_error_code 255

; 定义中断请求处理函数
irq_   0,    32 	; 电脑系统计时器
irq_   1,    33 	; 键盘
irq_   2,    34 	; 与 IRQ9 相接，MPU-401 MD 使用
irq_   3,    35 	; 串口设备
irq_   4,    36 	; 串口设备
irq_   5,    37 	; 建议声卡使用
irq_   6,    38 	; 软驱传输控制使用
irq_   7,    39 	; 打印机传输控制使用
irq_   8,    40 	; 即时时钟
irq_   9,    41 	; 与 IRQ2 相接，可设定给其他硬件
irq_  10,    42 	; 建议网卡使用
irq_  11,    43 	; 建议 AGP 显卡使用
irq_  12,    44 	; 接 PS/2 鼠标，也可设定给其他硬件
irq_  13,    45 	; 协处理器使用
irq_  14,    46 	; IDE0 传输控制使用
irq_  15,    47 	; IDE1 传输控制使用


[global isr_common_handler]     ;将该函数定义为全局的,方便调用
[extern isr_handler]    ;在c语言里定义的函数
;公共中断处理程序
isr_common_handler:
    pusha   ;一次压入eax,ecx,edx,ebx,old esp,ebp,esi,edi

    ;保存ds值
    mov ax,ds
    and eax,0x0000ffff
    push eax

    ;加载内核数据段描述符表
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp    ;把此时栈地址作为参数传入irs_handler函数(此时的地址就相当于是一个regs结构体的地址)
    call isr_handler
    add esp,4     ;释放上面压入的esp

    pop eax     ;恢复数据段描述符的值(就是上面压入的ds的值)
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    popa    ;恢复pusha压入的寄存器的值
    add esp,8   ;释放压入的中断号和错误代码

    iret

[global idt_flush]
idt_flush:
    mov eax,[esp+4]     ;将参数(idtr结构体的值)存入eax
    lidt [eax]          ;将eax的值存入idtr寄存器
    ret

; 中断请求处理函数
[global irq_common_handler]
[extern irq_handler]
irq_common_handler:
    pusha       ;保存寄存器eax,ecx,edx,ebx,old_esp,ebp,esi,edi

    mov eax,ds  ;保存数据段选择器的值
    push eax

    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    push esp    ;就相当于把当前的esp作为参数传递进去.
    call irq_handler    ;调用irq_handler函数.
    add esp,4

    pop eax     ;恢复段选择器的值(这个bug(pop写成了push),要是不写成汇编级调试,你该懂?!!).
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    popa
    add esp,8   ;清理栈空间
    iret