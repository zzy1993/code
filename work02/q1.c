#include <stdio.h>
#include <string.h>

void print_args(int argc, char **argv){
    for(int i = 0; i<argc; i++){
        printf("argv[%d] = '%s'\n", i, argv[i]);
    }
}

int get_args(char *in, char **argv, int max_args){
	char *word;
	char *sep = " ";
	int count = 0;
	for (word = strtok(in, sep); word; word = strtok(NULL, sep)){
		*(argv+count) = strdup(word);
		count += 1;
		if (count >= max_args){
			return count;
		}
	}
	int length = strlen(*(argv+count-1));
	*(*(argv+count-1)+length-1) = '\0';
	return count;
}

int main(){
	char s[200];
	char *argv[10];
	int argc;

	printf("Enter a string: ");
	fgets(s, sizeof s, stdin);

	argc = get_args(s,argv,10);
	print_args(argc, argv);
}