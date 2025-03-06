// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Simple Shell ~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Headers Including ~~~~~~~~~~~~~~~~~~~~~ //
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
char** commmand_parsing();
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions Declerations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int main()
{
	pid_t pid;
	int *status;
	while(1)
	{
		char** argv;	
		char *const envp[] = {NULL};
		char *const nargv[] = {NULL};

		printf("usr@shell >> "); 	// prompt
		argv = commmand_parsing();
		
		pid =  fork();
		if(pid < 0) 			// fork failed
		{
			//printf("Fork failed !!\n\r");
			return -1;			
		}
		else if(pid == 0) 		// child process
		{
			execve(argv[0], argv, envp);
			//printf("Execute command failed !!\n\r");
		}
		else if(pid > 0)		// parent process
			wait(status);	
	}		
	return 0;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions Definations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
char** commmand_parsing()
{	
	char **argv;
	char path[9] = {"/usr/bin/"};
	char buf[50];
	int argc = 0;
	fgets(buf, 50, stdin);
	
	// allocate number of argumrnts	
	for(int i = 0; i < 50; i++)
	{
		if((buf[i] != ' ') && (buf[i+1] == ' '|| buf[i+1] == '\n'))
			argc++;
		if(buf[i] == '\0')
			break;
	}

	// allocate memory space for all arguments
	argv = (char **)malloc((argc+1) * sizeof(char*));   

	// allocate memory space for each argument
	for(int i = 0; i < argc; i++)
	{
		argv[i] = malloc(50);
	}
	
	// fill path first
	for(int i = 0; i < 9; i++)
	{
		argv[0][i] = path[i];
	}
	
	// fill rest of arguments
	int len = strlen(argv[0]);
	int a = 0;
	int j = 0;
	for(int i = 0; i < 50; i++)
	{
		if(buf[i] == '\n')
		{
			break;
		}
		if(buf[i] == ' ')
		{
			if(a == 0) // special case (first arg is the path)
			{	
				argv[a][j+len] = '\0';
			}
			else
			{
				argv[a][j] = '\0';
		   	}			
			a++;
			j = 0;
		}
		else
		{
			if(a == 0) // special case (first arg is the path)
			{
				argv[a][j+len] = buf[i];
				j++;
			}
			else
			{
				argv[a][j] = buf[i];
				j++;
			}				
		}
	}

	argv[argc] = '\0';
	/*
	// test print
	for(int i = 0; i < argc; i++)
	{
		printf("%ld\n\r",strlen(argv[i]));
		printf("%s\n\r",argv[i]);
	}
	*/
	return argv;
}

/**************** functions notes *******************
 
 	- pid_t getpid(void); 
   	- pid_t getppid(void); 
	- char *fgets(char *s, int size, FILE *stream);
	- char *strcat(char *dest, const char *src);
	- pid_t fork(void);
	- size_t strlen(const char *s);
	- int execve(const char *pathname, char *const argv[], char *const envp[]);
	- pid_t wait(int *wstatus);

****************************************************/
