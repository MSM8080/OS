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
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Macros ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#define BUF_SIZE 50
#define LOCAL_VAR_SIZE 50
#define LOCAL_VAR_ARRAY_SIZE 50
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions Declerations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
char** command_parsing();
int exec_built_in_command(char **argv ,char** vars, int* v_idx);
void built_in_set(char** vars);
void built_in_export(char** argv, char** vars);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int main()
{
	pid_t pid;
	int *status;
	char** vars;       // local variables
	int vars_indx = 0; // local variables index

	// allocate memory space for all local variables
	vars = (char **)malloc((LOCAL_VAR_ARRAY_SIZE) * sizeof(char*));   

	// allocate memory space for each argument
	for(int i = 0; i < LOCAL_VAR_ARRAY_SIZE; i++)
	{
		vars[i] = malloc(LOCAL_VAR_SIZE);
	}

	// fill local variables array with nulls
	for(int i = 0; i < LOCAL_VAR_ARRAY_SIZE; i++)
	{
		vars[i] = NULL;
	}

	while(1)
	{
		char** argv;	// arguments array
		char *const envp[] = {NULL};

		printf("usr@shell >> "); 	// prompt
		
		argv = command_parsing();
		if (argv == NULL)
			continue;
		
		if(exec_built_in_command(argv,vars,&vars_indx) == 1) // check & execute built in command
			continue;
		
		pid =  fork();
		if(pid < 0) 			// fork failed
		{
			return -1;			
		}
		else if(pid == 0) 		// child process
		{
			execvp(argv[0], argv);
			//printf("Execute command failed !!\n\r");
		}
		else if(pid > 0)		// parent process
			wait(status);	
	}		
	return 0;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions Definations ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
char** command_parsing()
{	
	char **argv;
	char buf[BUF_SIZE];
	int argc = 0;
	fgets(buf, BUF_SIZE, stdin);
	
	// allocate number of argumrnts	
	for(int i = 0; i < BUF_SIZE; i++)
	{
		if((buf[i] != ' ') && (buf[i+1] == ' '|| buf[i+1] == '\n')) // word end by space or null
			argc++;
		else if(buf[i] == '\n') 	// end of buf
			break;;
	}

	// no command
	if(argc == 0)
		return NULL;

	// allocate memory space for all arguments
	argv = (char **)malloc((argc+1) * sizeof(char*));   

	// allocate memory space for each argument
	for(int i = 0; i < argc; i++)
	{
		argv[i] = malloc(BUF_SIZE);
	}
	
	// fill argv with arguments
	int a = 0;
	int j = 0;
	for(int i = 0; i < BUF_SIZE; i++)
	{
		if (buf[i] == '\n')  // end of buf
			break;
		else if(buf[i] == ' ' && buf[i-1] == ' ') 	// space between spaces
			continue;
		else if(buf[i] == ' ' && buf[i-1] != ' ') 	// space between words
		{
			argv[a][j] = '\0';
			a++;
			j = 0;
		}
		else   // characters
		{
			argv[a][j] = buf[i];
			j++;
		}
	}

	// end of argv is null
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int exec_built_in_command(char **argv ,char** vars, int* v_idx)
{
	if(strcmp(argv[0], "set") == 0) // set command
	{	
		built_in_set(vars);
	}
	else if(strcmp(argv[0], "export") == 0) // export command
	{	
		built_in_export(argv,vars);
	}
	else if (strchr(argv[0], '=')!= NULL) // is assign command 
	{
		if(*v_idx == LOCAL_VAR_ARRAY_SIZE)
		{
			printf("local variables array is full !! \n\r");		
		}
		else
		{
			vars[*v_idx] = argv[0];
			*v_idx += 1;
		}
	}
	else
	{
		return 0; // no built in command
	}	
	return 1; // built in command executed
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void built_in_set(char** vars)
{
	for(int i = 0; i < LOCAL_VAR_ARRAY_SIZE; i++)
	{
		if(vars[i] == NULL)
		{	
			break;
		}
		printf("Local var[%d]: %s \n\r",i,vars[i]);
	}
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void built_in_export(char** argv, char** vars)
{
	char* var;
	char* value;

	if(*argv[1] == '\n')
	{
		printf("write variable to export !! \n\r");
		return;
	}

	// search for variable
	for(int i = 0; i < LOCAL_VAR_ARRAY_SIZE; i++)
	{
		var = strtok(vars[i], "=");
		/* [strtok] In each subsequent call that should parse the same string, str must be NULL */
		value = strtok(NULL,"\n");
		if(strcmp(var,argv[1]) == 0)
		{
			setenv(var,value,1);	
			break;
		}
	}
}












/**************** functions notes *******************
 
 	- pid_t getpid(void); 
   	- pid_t getppid(void); 
	- char *fgets(char *s, int size, FILE *stream);
	- char *strcat(char *dest, const char *src);
	- int strcmp(const char *str_1, const char *str_2)
	- char *strchr(const char *str, int c)
	- char *strtok(char *str, const char *delim)
	- size_t strlen(const char *str)
	- pid_t fork(void);
	- size_t strlen(const char *s);
	- int execve(const char *pathname, char *const argv[], char *const envp[]);
	- pid_t wait(int *wstatus);
	- int execvp(const char *file, char *const argv[]);
	- int setenv(const char *name, const char *value, int overwrite);


****************************************************/
