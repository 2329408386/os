[global switch_to]

;保存当前线程的内容
switch_to:
    mov eax,[esp+4]

    ; 保存上一个线程的内容(主要是寄存器的保存)
    mov [eax+0], esp
    mov [eax+4], ebp
    mov [eax+8], ebx
    mov [eax+12], esi
    mov [eax+16], edi
    pushf
    pop ecx
    mov [eax+20], ecx

    mov eax,[esp+8]             ; 又是一天啊!!!!

    ; 恢复当前需要运行的进程
    mov esp,[eax+0]
    mov ebp,[eax+4]
    mov ebx,[eax+8]
    mov esi,[eax+12]            ;天啊啊啊啊啊啊!!!!!!, 汇编把eax写成ebx,然后调了一天!
    mov edi,[eax+16]
    mov eax,[eax+20]
    push eax
    popf

    ret