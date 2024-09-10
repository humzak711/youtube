
/*

    reflective so injection is where we load a shared object file containing
    a function with a constructor attribute into the memory of our running
    process to have the function executed once the shared object file has been 
    linked into our process

*/

#define _GNU_SOURCE // for macro's such as MFD_CLOEXEC
#include <unistd.h> // for disk file operations like write and fexecve
#include <sys/mman.h> // for memory file operations like memfd_create
#include <stdlib.h> // for exit codes like EXIT_FAILURE
#include <dlfcn.h> // for dynamic linker operations like dlopen
#include <stdio.h> // for input output operations and formatting too
#include "packed_so.h" // contains the unsigned char array with our binaries data

// test_so is an unsigned char array containing the binary data

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
        object to our process, we use RTLD_NOW here to resolve all symbols as
        soon as the shared object file has been linked resulting in our shared object
        executing its constructor once it is linked to this process
    */
    void* handle = dlopen(path_to_fd, RTLD_NOW);

    /* 
        The rest of this code will still run even if dlopen was successful, the reason for
        this is that our process will still be running this program, the shared object file
        was just linked to this process so we could acquire functions from it rather than overriding
        the process, therefore the memory space of this process will still hold our program, but the 
        shared object file will be unloaded from the process since it's reference count will now drop 
        down to zero
    */

   dlclose(handle); // cleanup the shared object
   ret = EXIT_SUCCESS;
done:
    close(anon_fd);
    return ret;
}