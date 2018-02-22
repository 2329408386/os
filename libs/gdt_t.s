[global gdt_flush]

gdt_flush:
    mov eax,[esp+4]     ;将参数传入eax变量
    lgdt [eax]      ;将eax所指向中的值设置成gdtr寄存器的值

    ;将数据段选择器设置为0x10(这里因为我的全局描述符表中,内核级数据段的位置就在0x10处)
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax

    ;将代码段选择器(cs)设置为0x8(在全局描述符表中,内核级代码段的位置在0x08处).
    jmp 0x08:end

end:
    ret     ;返回调用它的函数