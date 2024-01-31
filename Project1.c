#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

char *delim1 = " \n";
char *delim2 = "&";
int redirect = 0;
int flag;
int t8;
char *output_file = NULL;
void cd_command(char **argv);
void exit_command(char **argv);
void path_command(char **argv);
void execute(char **argv);
void printCommands(char **argv);
char** parseInputLine(char* cmd);
void print_error_message();
char* getNextCommand(char* cmd);
char** redirectFunction(char **argv, int *argc);

void print_error_message() {
    char error_message[] = "An error has occurred\n";
    ssize_t result = write(STDERR_FILENO, error_message, strlen(error_message));
if (result == -1) {
    // Handle the error if needed
}
    //write(STDERR_FILENO, error_message, strlen(error_message));
}


void printCommands(char **argv) {
    int i = 0;
    for (i = 0; argv[i] != NULL; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
}

void cd_command(char **argv){
    if (argv[1] == NULL){
         print_error_message();
        return;
    }
    if (argv[2] != NULL){
         print_error_message();
        return;
    }
    const char *newDirectory = argv[1];
    if (chdir(newDirectory) != 0) { 
    print_error_message();// Handle the error appropriately, e.g., print an error message and return.
}

}

void exit_command(char **argv){
      if (argv[1] != NULL) {
      	print_error_message();
       	return;
    }
	exit(0);
 }

char *shell_path = "/bin";
void path_command(char **argv) {
    if (argv[1] == NULL) {
        setenv("PATH", "", 1);
       // printf("Path command executed\n");
        return;
    }

    // Create a new buffer to hold the updated path
    char new_path[256];  // Adjust the buffer size as needed
    strcpy(new_path, argv[1]);

    int i;
    for (i = 2; argv[i] != NULL; i++) {
        strcat(new_path, ":");
        strcat(new_path, argv[i]);
    }

    // Update shell_path with the new_path
    setenv("PATH", new_path, 1);
    //printf("Path command executed with arguments\n");
}



char* getNextCommand(char* cmd){
    char* token = strtok(cmd, "&");
    return token;
}	

char** redirectFunction(char **argv, int *argc)
{
	redirect = 0;t8 =0;
	int i;
         for (i = 0; i < *argc; i++)
            {
                if (strcmp(argv[i], ">") == 0)
                {	
                    if(*argc - i > 2 || strcmp(argv[0], ">") == 0 )
		    {
		    print_error_message();
		    t8 = 1;
		    break;		
		    }	    
		     if (i + 1 < *argc)
                    {
                        redirect = 1;
                        output_file = argv[i + 1];
                        argv[i] = NULL;
                        break;
                    }
                    else
                    {
                         print_error_message();
			 t8 = 1;
                        redirect = 0;
                        break;
                    }
                }
            }
	 return argv;
}


void execute(char **argv)
{		
    if (strcmp(argv[0], "exit") == 0)
    {
        exit_command(argv);
    }
    else if (strcmp(argv[0], "cd") == 0)
    {
        cd_command(argv);
    }
    else if (strcmp(argv[0], "path") == 0)
    {
        path_command(argv);
    }
    else
    {
	char path_copy[256];  //testing
    	strcpy(path_copy, getenv("PATH"));  //testing	
        //char *path = getenv("PATH"); testing
        char *saveptr;
	char *token = strtok_r(path_copy, ":", &saveptr);     //    testing _copy added
	while (token != NULL)
        {
            char executable_path[256]; // Adjust the buffer size as needed
            snprintf(executable_path, sizeof(executable_path), "%s/%s", token, argv[0]);
		
	    
            // Check if the constructed path is executable
            if (access(executable_path, X_OK) == 0)
            {
		    
                pid_t child_pid = fork();
                if (child_pid == -1)
                {
                    perror("fork");
                    print_error_message();
                    return;
                }
                else if (child_pid == 0)
                {
                    char *command_argv[256]; // Adjust the buffer size as needed
                    int i = 0;
                    command_argv[i++] = executable_path;

                    for (int j = 1; argv[j] != NULL; j++)
                    {
                        command_argv[i++] = argv[j];
                    }
                    command_argv[i] = NULL;

                    if (redirect == 1)
                    {
                        int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                        if (output_fd == -1)
                        {
                            perror("open");
                            print_error_message();
                            exit(EXIT_FAILURE); // Exit the child process with an error code.
                        }
                        dup2(output_fd, STDOUT_FILENO);
                        close(output_fd);
                    }

                    execv(executable_path, command_argv);

                    // If execv fails, handle the error here
                    perror("execv");
                    print_error_message();
                    exit(EXIT_FAILURE); // Exit the child process with an error code.
                }
                else
                {
                    wait(NULL);
                }
                return; // We found and executed the command, so we're done.
            }
		
            token = strtok_r(NULL, ":", &saveptr);
	   
        }

        // If we reach here, the command was not found in any directory in the PATH.
        print_error_message();
        return;
    }
}


char** parseInputLine(char* cmd)
{	
	int i = 0;
	int argc = 0;
	char *cmd_cpy = NULL, *token = NULL;
	cmd_cpy = strdup(cmd);

        token = strtok(cmd, delim1);

        while(token)
        {
                token = strtok(NULL, delim1);
                argc++;
        }
	
	char **argv = NULL;
        argv = malloc(sizeof(char *)*(argc+1));

        token = strtok(cmd_cpy, delim1);

        while(token)
        {
                
		argv[i] = token;
                token = strtok(NULL, delim1);
                i++;	

        }
        argv[i] = NULL;
	return argv;
}


int main(int argc, char *argv[])
{
	char *new_path = "/bin:/usr/bin:/usr/local/bin"; // Example path with necessary directories
	setenv("PATH", new_path, 1);
	if(argc == 2){
	char **args = NULL;
	char *trimmed_line = NULL;
	FILE *batchFile = fopen(argv[1], "r");
        if (batchFile == NULL) {
            
	     print_error_message();
            exit(1);}
	char *line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, batchFile)) != -1) {
            trimmed_line = line;
            while (*trimmed_line && (*trimmed_line == ' ' || *trimmed_line == '\t' || *trimmed_line == '\n')) {
            trimmed_line++;}
	    size_t line_len = strlen(trimmed_line);
   while (line_len > 0 && (trimmed_line[line_len - 1] == ' ' || trimmed_line[line_len - 1] == '\t' || trimmed_line[line_len - 1] == '\n'))    {
        trimmed_line[--line_len] = '\0';
    }
   	
	    char *newline = strchr(trimmed_line, '\n');
	    char *cmd_cpy = NULL, *token = NULL, *cmd_cpy2 = NULL;
	    char *delim1 = " \n";
	    int argcount = 0;
    	    char **argh = NULL;
	    char *str1;
            char *saveptr1;
            flag = 0;	    
            if (newline != NULL) {
                *newline = '\0';
            }
	    cmd_cpy = strdup(trimmed_line);
            cmd_cpy2 = strdup(trimmed_line);
	     token = strtok(cmd_cpy, delim1);
	     while(token)
         	{
                token = strtok(NULL, delim1);
                argcount++;

                if(token && strcmp(token, "&") == 0)
                        flag = 1;
        	}
	     if(flag == 1)
        	{
        	int j;
        	for (str1 = cmd_cpy2, j = 1; ; j++, str1 = NULL) {
               token = strtok_r(str1, "&", &saveptr1);
               if (token == NULL)
                   break;
               argh = malloc(sizeof(char *) * argcount);  //test comment
                argh = parseInputLine(token);
               if (strcmp(argh[0], "cd") == 0 && argh[1] != NULL) {
                          execute(argh);
                          free(argh);
                }else{
		pid_t child_pid = fork();
               if(child_pid == 0)
                {

	     args = malloc(sizeof(char *)*argcount);
	     args = parseInputLine(token);
	     if (args[0] == NULL) {
        continue;  // Skip empty lines
    }
	    execute(args);
   		free(args);	    // comment try
        exit(EXIT_SUCCESS);
        }
               else
                       wait(NULL);
        }
        }}
      else{
        args = malloc(sizeof(char *)*argcount);
        args = parseInputLine(cmd_cpy2);

         argh = redirectFunction(args, &argcount);
         
	 if (args[0] == NULL) {
        continue;  // Skip empty lines
    }
	 if(t8 != 1)
	 execute(args);
         free(args);
        }
        free(cmd_cpy);
        free(cmd_cpy2);
	}
	free(line);	
        fclose(batchFile);
 	}
	else{	
   while(1){
        char *cmd = NULL, *cmd_cpy = NULL, *token = NULL, *cmd_cpy2 = NULL;
        char *delim1 = " \n";
	// unused failed char *delim2 = "&";
        size_t n = 0;
        int argc = 0;
        char **argv = NULL, **argd = NULL;// **argg = NULL;
	char *str1;
        char *saveptr1;
	flag = 0;
	
        printf("dash> ");
       if(getline(&cmd, &n, stdin) == -1)
                break;
 			
	cmd_cpy = strdup(cmd);
	cmd_cpy2 = strdup(cmd);

        token = strtok(cmd, delim1);
	
        while(token)
        {
                token = strtok(NULL, delim1);
                argc++;

		if(token && strcmp(token, "&") == 0)
			flag = 1;
        }
	if(flag == 1)
	{
	int j;
	for (str1 = cmd_cpy, j = 1; ; j++, str1 = NULL) {
               token = strtok_r(str1, "&", &saveptr1);
               if (token == NULL)
                   break;
	       argd = malloc(sizeof(char *) * argc);  //test comment
        	argd = parseInputLine(token);	
	       if (strcmp(argd[0], "cd") == 0 && argd[1] != NULL) {
          		  execute(argd);
			  free(argd);
            	}else{
	       pid_t child_pid = fork();
	       if(child_pid == 0)
		{
	
	argv = malloc(sizeof(char *)*argc);
        argv = parseInputLine(token);  

	// argg = redirectFunction(argv, &argc); //comment		
	execute(argv);
	free(argv);
	exit(EXIT_SUCCESS);   	 
	}
	       else 
		       wait(NULL);
	}
	}}
	else{	
	argv = malloc(sizeof(char *)*argc);
        argv = parseInputLine(cmd_cpy);
	
	 argd = redirectFunction(argv, &argc);
	 if(t8 != 1)
	 execute(argv);
	 free(argv);
	}
	
        free(cmd);
        free(cmd_cpy);
	free(cmd_cpy2);

   }}
        return 0;

}
