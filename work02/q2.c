#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv){

    int fds[2];
    int pid = 0;
    if (pipe(fds) == -1){
        perror("pipe");
        return 1;
    }
    int fds2[2];
    if (pipe(fds2) == -1){
        perror("pipe2");
        return 1;
    }
    int ret = fork();
    if (ret < 0){
        perror("fork");
        return 1;
    }
    else if (ret == 0){
        int ret2 = fork();
        if (ret2 < 0){
            perror("fork2");
            return 1;
        }
        else if (ret2 == 0){
            close(fds2[0]);
            pid = (int)getpid();
            //printf("grandchild is %d\n", pid);
            //close(fds2[0]);
            write(fds2[1], &pid, sizeof(pid));
            close(fds2[1]);

            close(fds[1]);
            close(0);
            dup(fds[0]);
            char *argv[2];
            argv[0] = "wc";
            argv[1] = NULL;
            execvp(argv[0], argv);
        }
        else {
            //printf("child\n");
            close(fds2[0]);
            close(fds2[1]);
            close(fds[0]);
            close(1);
            dup(fds[1]);
            char *argv[3];
            argv[0] = "ls";
            argv[1] = "-l";
            argv[2] = NULL;
            execvp(argv[0], argv);
        }
    }
    else {
        close(fds2[1]);
        int pidChild = wait(NULL);
        close(fds[1]);
        read(fds2[0], &pid, sizeof(pid));
        close(fds2[0]);
        //int pidChild = wait(NULL);
        printf("Process %d finished\n", pidChild);
        printf("Process %d finished\n", pid);
    }
    return 0;
}
