global _start ; global _start declares _start as our programs entry point

; x86 calling convention:
; rax - holds syscall number
; rdi - first argument
; rsi - second argument
; rdx - third argument
; r10 - fourth argument
; r8 - fifth argument
; r9 - sixth argument

section .text ; .text is the section where our code is
_start: ; declare _start and the instructions under it which will be executed when we execute the program

    ; output Hello world!
    mov rax, 1 ; 1 is the syscall number for write
    mov rdi, 1 ; load rdi with 1 to tell write to output to stdout
    mov rsi, msg ; load rsi with msg which is our variable containing hello world!
    mov rdx, msglen ; load rdx with the length of what we are outputting

    syscall ; call the system call to output Hello World!

    ; exit the program smoothly to prevent segfault
    mov rax, 60 ; 60 is the system call for exit
    mov rdi, 0 ;  load 0 into rdi for EXIT_SUCCESS
    syscall ; call the system call to exit the program


section .rodata ; .rodata is the read only data section used to hold constant data unchanged during program execution
msg: db "Hello world!", 10 ; declare a sequence of bytes (a string) variable called msg which holds the value Hello World!, 10 is  to represent a newline character
msglen equ $ - msg ; get the length of msg and store the length in msglen
