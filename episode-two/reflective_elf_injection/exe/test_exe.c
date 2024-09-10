#include <stdio.h>

void main(int argc, char** argv) {

    /* 
        if unable to get this processes executables filepath then it's running in memory
        argv[0] contains the executables filepath and we will be able to fopen it if its 
        on disk but we wont be able to fopen it if it is in memory
    */
    FILE* file = fopen(argv[0], "r"); 
    if (!file) printf("reflective ELF injection with an executable\n");
    if (file){
        printf("running on disk not in memory");
        fclose(file);
    } 
}