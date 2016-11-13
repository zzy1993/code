#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10
#define PATH_MAX 64
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>

struct SubCommand{
	char* line;
	char* argv[MAX_ARGS];
};

struct Command{
	struct SubCommand sub_commands[MAX_SUB_COMMANDS];
	int num_sub_commands;
	char* stdin_redirect;
	char* stdout_redirect;
	char* stdout_redirect_a;
	int background;
};

// Read full

void ReadArgs(char *proc, char **argv, int size){
	char* temp = strdup(proc);
	char* word;
	char* sep = " ";
	int count = 0;
	for (word = strtok(temp, sep); word; word = strtok(NULL, sep)){
		*(argv+count) = strdup(word);
		if (count == size-1){
			break;
		}
		count += 1;
	}
	*(argv+count) = NULL;
}

// Read subcommands from command.

void ReadCommand(char* line, struct Command* command){
	char* temp = strdup(line);
	char* proc;
	char* sep = "|";
	int count = 0;
	for (proc = strtok(temp, sep); proc; proc = strtok(NULL, sep)){
		command->sub_commands[count].line = strdup(proc);
		count += 1;
	}
	command->num_sub_commands = !(count < MAX_SUB_COMMANDS)? MAX_SUB_COMMANDS: count;
	int i;
	for (i = 0; i < command->num_sub_commands; i++){
		ReadArgs(command->sub_commands[i].line, command->sub_commands[i].argv, MAX_ARGS);
	}
}

// Read [1] command [2] rediStdIn [3] rediStdOut [4] backgnd from full input.

bool ReadRedirectsAndBackground(struct Command *command){
	char line[200];
	char result[PATH_MAX];
	char* path = getcwd(result, PATH_MAX);
	char* user = getlogin();
	if(user == NULL){
		user = "";
	}
	printf("%s@Shell %s$ ", user, path);
	fgets(line, sizeof(line), stdin);
	line[strlen(line)-1] = '\0';
	if(strlen(line)==0){
		return true;
	}
	command->stdin_redirect = NULL;
	command->stdout_redirect = NULL;
	command->background = 0;
	char* ptrFirst = &line[0];
	char* ptrStr = line;
	char* temp;
	int i;
	for (i = strlen(line)-1; i >= 0; i--){
		switch(*(ptrFirst+i)){
			//redirect input match
			case '<':	//'<', stdin
				temp = ptrFirst+i+1;
				while (*temp == ' '){
					temp += 1;
				} 
				command->stdin_redirect = strdup(temp);
				*(ptrFirst+i) = '\0';
				break;
			//redirect output (write/append) match
			case '>':	//'>', stdout
				if (*(ptrFirst+i-1) == '>'){
					temp = ptrFirst+i+1;
					while (*temp == ' '){
						temp += 1;
					} 
					command->stdout_redirect_a = strdup(temp);
					*(ptrFirst+i-1) = '\0';
				}
				else{
					temp = ptrFirst+i+1;
					while (*temp == ' '){
						temp += 1;
					} 
					command->stdout_redirect = strdup(temp);
					*(ptrFirst+i) = '\0';
				}
				break;
			//background match
			case '&':
				if (i == strlen(line)-1){
					command->background = 1;
					*(ptrFirst+i) = '\0';
				}
				break;
			default:
				break;
		}
	}
	ReadCommand(line, command);
	return false;
}

/*
this is the newly added function that processes the execution of the shell commands
this function right now only supports ls -l, cat text_file.txt, ls -l|wc, No any input
or output redirection is implemented in this case. Some problems still remain: 
1. as you can see from the code, only 1 pipe is presented, and only 2 sub-commands are supported
how to create N-1 pipes if you have N sub-commands? I just don't understand the last paragraph of
the shell project. 
Apart from that, things should be fairly easy. Sample codes are presented on Unit 2 of regarding 
standard input/output redirection.   
*/


void ExecuteCommand(struct Command *command){
	// create N-1 pipes
	int fds[command->num_sub_commands-1][2];
	int i;
	for (i = 0; i < command->num_sub_commands-1; i++){
		//printf("pipe: %d\n", i);
		if (pipe(fds[i]) > 0){
			perror("pipe fds");
		}
	}
	//standby pipe for pid transfer
	int fb[2];
	// create the pipe 
	if (pipe(fb)>0){
		perror("pipe fb");
	}
	bool isBgd = true;
	int pid;	//pid transfer use
	for (i = 0; i < command->num_sub_commands; i++){
		int ret = fork();
		if (ret < 0){
			perror("fork");
		}

		// Child [extend]

		else if (ret == 0){
			//printf("loop: %d\n", i);
			if(command->num_sub_commands > 1){
				if (i == 0){
					// case of first pipe
					close(fds[i][0]);
					close(1);
					dup(fds[i][1]);
				}
				else if (i == command->num_sub_commands-1){
					// case of last pipe
					close(fds[i-1][1]);
					close(0);
					dup(fds[i-1][0]);
				}
				else{
					// read from pipe i-1
					close(fds[i-1][1]);
					close(0);
					dup(fds[i-1][0]);
		            // write to pipe i
					close(fds[i][0]);
					close(1);
					dup(fds[i][1]);
				}
			}

			// Bgd
			if (command->background == 1){
				pid = getpid();
				close(fb[0]);
				write(fb[1], &pid, sizeof(pid));
				command->background=0;
			}

			if(command->stdin_redirect != NULL && i == 0){  //input redirection detected
				close(0);                  //close default
				if(open(command->stdin_redirect, O_RDONLY)<0){
					printf("%s: file not found\n",command->stdin_redirect);
					exit(1);
				}
				command->stdin_redirect=NULL;	//chear up
			}

			if(command->stdout_redirect!=NULL && i == command->num_sub_commands-1)	//output redirection (w) detected
			{
				close(1);	//close default
				if(open(command->stdout_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0660)<0){
					printf("%s: cannot create file\n",command->stdout_redirect);
					exit(1);
				}
				command->stdout_redirect=NULL;	//clear up
			}else if(command->stdout_redirect_a!=NULL && i == command->num_sub_commands-1)	//output redirection (append) detected
			{
				close(1);	//close default
				if(open(command->stdout_redirect_a, O_WRONLY | O_APPEND, 0660)<0){
					printf("%s: cannot create file\n",command->stdout_redirect_a);
					exit(1);
				}
				command->stdout_redirect_a=NULL;	//clear up
			}

			// Exec
			if (strcmp(command->sub_commands[i].argv[0], "cd") == 0){
				char *directory = command->sub_commands[i].argv[1];
				if (chdir (directory) == -1){
					printf("%s: directory not found\n",command->sub_commands[i].argv[0]);
					exit(1);
				}
			}else{
				// set environment variable
				char* ptrStr = command->sub_commands[i].argv[0];
				char** ptrLine = command->sub_commands[i].argv;
				char* var = NULL;
				char* value = NULL;
				int j;
				for (j=0; j<strlen(ptrStr); j++){
					if(ptrStr[j]=='='){
						value = ptrStr+j+1;
						ptrStr[j]='\0';
						var = ptrStr;
					}
				}
				if(var!=NULL || value!=NULL){
					setenv(var, value, 1);
					ptrLine += 1;
					ptrStr = ptrLine[0];
				}
				// execute program
				if (execvp(ptrStr, ptrLine) == -1){
					printf("%s: command not found\n",command->sub_commands[i].argv[0]);
					exit(1);
				}
			}
		}

		// Parent [main]
		else{
			if(command->num_sub_commands > 1)
				if(i < command->num_sub_commands-1)
					close(fds[i][1]);
			// if in background running
			if(command->background == 1){
		     	//write end close, prepare to read
				close(fb[1]);
				read(fb[0],&pid,sizeof(pid));
				printf("[%d]\n",pid);
				command->background=0;
				isBgd=false;
			}
			else{
				if (isBgd == true){	//no background
					waitpid(ret,NULL,0);	//then wait 
				}
			}
		}
	}
}

int main(){
	while(1){
		struct Command* command = malloc(sizeof(struct Command));
		bool isNextLine = ReadRedirectsAndBackground(command);
		if(isNextLine){
			continue;
		}
	    ExecuteCommand(command);
	    free(command);
	}
	return 0;
}