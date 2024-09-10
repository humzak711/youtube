#include <stdio.h>

/*
    when we call dlopen on the shared object file, this function is
    resolved and executed straight away, the reason for this is because
    since the function has the constructor attribute which tells the gcc
    compiler that it wants the function to execute automatically as soon as
    the shared object file has managed to be linked to a process

    in the case that a function with the constructor attribute is
    executed within an executable file, the function is executed before
    the main function, so this attribute is very useful for us
*/
__attribute__((constructor)) 
void TwofishSerpent() {
    printf("reflective ELF injection with a shared object file \n");
}