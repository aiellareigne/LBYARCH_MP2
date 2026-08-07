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
    cvtss2si r10d, xmm0

    test    r10d, r10d
    jge     .check_max
    xor     r10d, r10d
    jmp     .store

.check_max:
    cmp     r10d, 255
    jle     .store
    mov     r10d, 255

.store:
    mov     [rcx], r10b

    add     rdx, 4
    inc     rcx

    dec     r9
    jnz     .loop

.done:
    ret
