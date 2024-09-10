
/*

    reflective elf injection with an executable is where we
    load an executable file into memory and then execute it in
    the memory of our own currently running process

*/

#define _GNU_SOURCE // for macro's such as MFD_CLOEXEC
#include <unistd.h> // for disk file operations like write and fexecve
#include <sys/mman.h> // for memory file operations like memfd_create
#include <stdlib.h> // for exit codes like EXIT_FAILURE
#include "packed_exe.h"

int main() {

    /* 
        create a file descriptor in memory, this is called an anonymous file descriptor, 
        we use MFD_CLOEXEC here so that the file descriptor is closed once the file held 
        by the file descriptor is executed to ensure proper cleanup
    */
    int anon_fd = memfd_create("", MFD_CLOEXEC);
    if (anon_fd < 0) return EXIT_FAILURE;

    // write the binary into the file held by the file descriptor
    if (write(anon_fd, test_exe, sizeof(test_exe)) < sizeof(test_exe) || lseek(anon_fd, 0, SEEK_SET) == -1) goto failed;

    /*
        execute the file held by the file descriptor with fexecve

        fexecve is a function used to directly execute files held 
        by file descriptors
    */
    char *const argv[] = {NULL};
    char *const envp[] = {NULL};
    fexecve(anon_fd, argv, envp); 

    /*
        if fexecve managed to execute the binary successfully then anything below here 
        won't be executed because this process will now be running the binary we just 
        executed in memory  
    */
failed:
    close(anon_fd);
    return EXIT_FAILURE;
}