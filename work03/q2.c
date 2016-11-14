#define MAX_SUB_COMMANDS 5
#define MAX_ARGS 10
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//cd ~/Desktop/ir/ | python as3-1.py

struct SubCommand{
	char* line;
	char* argv[MAX_ARGS];
};

struct Command{
	struct SubCommand sub_commands[MAX_SUB_COMMANDS];
	int num_sub_commands;
};

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
	*(argv+count) = "\0";
}

void ReadCommand(char* line, struct Command* command){
	char* temp = strdup(line);
	char* proc;
	char* sep = "|";
	int count = 0;
	for (proc = strtok(temp, sep); proc; proc = strtok(NULL, sep)){
		command->sub_commands[count].line = strdup(proc);
		count += 1;
	}
	command->num_sub_commands = !(count<MAX_SUB_COMMANDS)?MAX_SUB_COMMANDS:count;
	for (int i = 0; i < command->num_sub_commands; i++){
		ReadArgs(command->sub_commands[i].line, command->sub_commands[i].argv, MAX_ARGS);
	}
}

void PrintArgs(char **argv){
	int i = 0;
	char **temp = argv;
    while(**temp != '\0'){
        printf("argv[%d] = '%s'\n", i, *temp);
        i += 1;
        temp += 1;
    }
}

void PrintCommand(struct Command* command){
	for (int i = 0; i < command->num_sub_commands; i++){
		printf("process[%d]:\n", i);
		PrintArgs(command->sub_commands[i].argv);
	}
}

int main(){
	char line[200];
	printf("Enter a string: ");
	fgets(line, sizeof(line), stdin);
	line[strlen(line)-1] = '\0';
	struct Command* command = malloc(sizeof(struct Command));
	ReadCommand(line, command);
    PrintCommand(command);
    free(command);
    return 0;
}