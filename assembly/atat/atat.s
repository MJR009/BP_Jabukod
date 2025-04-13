# as atat.s -o atat.o && ld -o atat atat.o && ./atat

# DEBUG
# as -g atat.s -o atat.o && ld -o atat atat.o && ./atat
# gdb ./atat
# tui enable
# layout regs

.data
hello:
    .asciz "Hello world!\n"
hello_len:
    .long .-hello

.text
.globl _start

_start:
#    movabs $1, %rdi # 64 bit $imm
    movq $1, %rdi # 32 bit $imm
    lea (hello), %rsi
    movq hello_len, %rdx

    movq $1, %rax
    syscall

    xor %rdi, %rdi

    movq $60, %rax
    syscall
