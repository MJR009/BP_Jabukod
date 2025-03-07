# as atat.s -o atat.o && ld -o atat atat.o && ./atat

.data
hello:
    .asciz "Hello world!\n"
hello_len:
    .long .-hello

.text
.globl _start

_start:
    movq $1, %rdi
    lea (hello), %rsi
    movq hello_len, %rdx

    movq $1, %rax
    syscall

    xor %rdi, %rdi

    movq $60, %rax
    syscall
