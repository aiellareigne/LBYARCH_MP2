section .rodata
    align 16
    scale_factor: dd 255.0

section .text
    global imgCvtGrayFloatToInt

imgCvtGrayFloatToInt:
    movsxd  rax, edi
    movsxd  r8, esi
    imul    rax, r8

    test    rax, rax
    jle     .done

    mov     r9, rax
    movss   xmm1, [rel scale_factor]

.loop:
    movss   xmm0, [rdx]
    mulss   xmm0, xmm1
    cvtss2si eax, xmm0

    test    eax, eax
    jge     .check_max
    xor     eax, eax
    jmp     .store

.check_max:
    cmp     eax, 255
    jle     .store
    mov     eax, 255

.store:
    mov     [rcx], al

    add     rdx, 4
    inc     rcx

    dec     r9
    jnz     .loop

.done:
    ret
