#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    int pid = fork();
    if (pid < 0){
        fprintf(stderr, "Fork failed");
        exit(1);
    }
    else if (pid == 0){
        int num;
        printf("%d", getpid());
        printf("Enter a number: ");
        scanf("%d", &num);
        exit(num);
    }
    else {
        int status;
        wait(&status);
        printf("Child exited with status: %d", WEXITSTATUS(status));
    }
}
