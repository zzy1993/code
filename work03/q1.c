#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void PrintArgs(char **argv){
	int i = 0;
	char **temp = argv;
    while(**temp != '\0'){
        printf("argv[%d] = '%s'\n", i, *temp);
        i += 1;
        temp += 1;
    }
}
void ReadArgs(char *in, char **argv, int size){
	char *word;
	char *sep = " ";
	int count = 0;
	for (word = strtok(in, sep); word; word = strtok(NULL, sep)){
		*(argv+count) = strdup(word);
		if (count == size-1){
			*(argv+count) = "\0";
			return;
		}
		count += 1;
	}
	int length = strlen(*(argv+count-1));
	*(*(argv+count-1)+length-1) = '\0';
	*(argv+count) = "\0";
	return;
}
int main(){
	char s[200];
	char s2[20];
	printf("Enter a string: ");
	fgets(s, sizeof s, stdin);
	int size;
	while(1){
		printf("Enter a size (not smaller than 1): ");
		fgets(s2, sizeof s2, stdin);
		size = atoi(s2);
		if (size >= 1){
			break;
		}
	}
	char **argv = malloc(size*sizeof(char*));
	ReadArgs(s, argv, size);
    PrintArgs(argv);
    free(argv);
    return 0;
}