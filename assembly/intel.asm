; nasm -f elf64 intel.asm && ld -m elf_x86_64 -o intel intel.o && ./intel

; DEBUG
; nasm -f elf64 -g -F dwarf intel.asm && ld -m elf_x86_64 -o intel intel.o && ./intel
; gdb ./intel
; tui enable
; layout regs

    SECTION .rodata
hello:   db "Hello world!",10,0
hello_len:   equ $-hello

    SECTION .text
    global _start
_start:
    mov rdi, 1 ; 32 bit move -> more efficient encoding, the 1 is sign extended into rdx
    ; mov rdi, strict qword 1 ; 64 bit move
    ; mov rax, 0123456789abcdefh ; -//-
    lea rsi, [hello]
    mov rdx, hello_len

    mov rax, 1
    syscall

    xor rdi, rdi

    mov rax, 60
    syscall
