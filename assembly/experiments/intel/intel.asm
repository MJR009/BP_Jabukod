; nasm -f elf64 intel.asm && ld -m elf_x86_64 -o intel intel.o && ./intel

    SECTION .rodata
hello:   db "Hello world!",10
hello_len:   equ $-hello


    SECTION .text
    global _start
    bits 64
_start:
    mov rdi, 1
    lea rsi, [hello]
    mov rdx, hello_len

    mov rax, 1
    syscall

    xor rdi, rdi

    mov rax, 60
    syscall
