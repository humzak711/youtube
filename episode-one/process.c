#include <stdio.h>
#include <unistd.h> // for process and pid operations

void main() {

	// getpid() is used to get our own process id 
	printf("parent: My process id is %d\n", getpid());
	
	// fork() is to create a new child process and return it's process id 
	pid_t cpid = fork();

	switch(cpid) {
		case -1:
		    perror("fork"); // perror is to print error, good error handling
		    return;

		// child process will execute code at case 0:
		case 0:
	            // getppid() is used to get our parent process id
		    printf("child: My process id is %d and my parent process id is %d\n", getpid(), getppid()); 

		// parent process will execute code at default:
		default:
	}
	return;
}
