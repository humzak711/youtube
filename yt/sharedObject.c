#define _GNU_SOURCE // to declare macro's such as RTLD_NEXT
#include <dlfcn.h> // for dynamic linker operations
#include <dirent.h> // for types which I need for this readdir hook
#include <stdlib.h>
#include <stdio.h>

// make a typedef for the function signature for the original readdir function 
typedef struct dirent *(*readdir_t)(DIR *dirp);

// pointer to the original readdir function
readdir_t OG_readdir = NULL;

/* 
	the hook for readdir, when the hooked binary makes a call to readdir,
	then this is the function which will be called
*/
struct dirent *readdir(DIR *dirp) {

	/*
		make a call to dlsym with RTLD_NEXT so that we can get the pointer
		to the original readdir function from the dynamic linker, the dlsym function 
		originally returns the type void * so we will use a type cast to make sure 
		the output has the correct function signature
	*/
	if (!OG_readdir) OG_readdir = (readdir_t)dlsym(RTLD_NEXT, "readdir");
	printf("Shared object hooked into readdir\n");

	/*
		call the original readdir function so that we can get it's
		output and return it to the user, if the original readdir was
		not able to be found by dlsym then we will return NULL here instead
	*/
	return OG_readdir ? OG_readdir(dirp) : NULL; 
}

/*
	__attribute__((constructor)) is used to execute a function from the shared object file
        once it has loaded into a binary 	
*/
__attribute__((constructor)) 
void in_the_front_door() {
	printf("Shared object loaded into the binary\n");
}

/*
        __attribute__((destructor)) is used to execute a function from the shared object file
        once it has been unloaded from a binary
*/
__attribute__((destructor))
void out_the_back_door() {
	printf("Shared object unloaded from the binary\n");
}

