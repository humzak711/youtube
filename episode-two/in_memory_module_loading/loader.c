
/*

    In memory module loading is where we dynamically load a shared 
    object file into the memory of our own process to acquire functions 
    or variables from it which you can execute from your program

*/

#define _GNU_SOURCE // for macro's such as MFD_CLOEXEC
#include <unistd.h> // for disk file operations like write and fexecve
#include <sys/mman.h> // for memory file operations like memfd_create
#include <stdlib.h> // for exit codes like EXIT_FAILURE
#include <dlfcn.h> // for dynamic linker operations like dlopen
#include <stdio.h> // for input output operations and formatting too
#include "packed_so.h"

/* 
    typedef holding the function signature of the function that
    we want to get from the shared object so that we can cast the 
    pointer to the target function into its function signature
    since dlsym will return it as a void pointer
*/
typedef void (*target_function_t)(void);

int main() { 

    int ret = EXIT_FAILURE;

    // create a file descriptor in memory 
    int anon_fd = memfd_create("", MFD_CLOEXEC);
    if (anon_fd < 0) return ret;

    // write the binary into the file held by the file descriptor
    if (write(anon_fd, test_so, sizeof(test_so)) < sizeof(test_so) || lseek(anon_fd, 0, SEEK_SET) == -1) goto done;

    /* 
        format the path to the file descriptor that is in memory 
        
        (our shared object file) which will be in /proc/self/fd which 
        holds all file descriptors in use by this process and inherited 
        from parent processes 

        /proc/self is the directory containing information about our running process,
        self in this context refers to our processes process ID
    */
    char path_to_fd[256];
    snprintf(path_to_fd, sizeof(path_to_fd), "/proc/self/fd/%d", anon_fd);

    /*
        call dlopen to tell the dynamic linker to dynamically link the shared
        object to our process

        dlopen will return a handle, the handle will be used to identify this shared
        object file from our program since our program may be linked to multiple different
        shared objects, we need to be able to have a way to identify each one
    */
    void* handle = dlopen(path_to_fd, RTLD_NOW);
    if (handle) {

        /*
            make a call to dlsym, the first argument we pass the handle to our
            opened shared object so the dynamic linker knows which shared object to get
            the function from, then in the second argument we pass the name of the function 
            we want to get

            since dlsym returns the type void* we must cast the type of the returned function
            pointer to it's expected function signature using the typedef for it which we wrote
            at the start of this code
        */
        target_function_t target_func = (target_function_t)dlsym(handle, "target_function");

        target_func(); // now we can execute the function
    }

    dlclose(handle); 
    ret = EXIT_SUCCESS;
done:
    close(anon_fd);
    return ret;
}