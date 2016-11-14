#import <stdlib.h>
#import <stdio.h>
#import <unistd.h>
int main(){
	//Parent to child
	int fdsPC[2];
	//Child to parent
	int fdsCP[2];
	if(pipe(fdsPC) == -1){
		printf("Error in fdsPC\n");
	}
	if(pipe(fdsCP) == -1){
		printf("Error in fdsCP\n");
	}
	int ret = fork();
	//After fork
	if(ret == 0){
		//child
		close(fdsPC[1]);
		close(fdsCP[0]);
		int pid = getpid();
		//C->P
		for(int i = 0; i < 5; i++){
			//fflush(stdout);
			read(fdsPC[0], &pid, sizeof(pid));
			//printf("child receives %d\n", pid);
			printf("%d, Child\n", i + 1);
			write(fdsCP[1], &pid, sizeof(pid));
			//printf("child sends %d\n", pid);
		}
		return 0;
	}
	else if (ret > 0){
		//parent
		close(fdsCP[1]);
		close(fdsPC[0]);
		int pid = getpid();
		//P->C
		for (int i = 0; i < 5; i++){
			//fflush(stdout);
			write(fdsPC[1], &pid, sizeof(pid));
			//printf("parent sends %d\n", pid);
			printf("%d, Parent\n", i + 1);
			read(fdsCP[0], &pid, sizeof(pid));
			//printf("parent receives %d\n", pid);
		}
		wait(NULL);
		return 0;
	}
	return -1;
}