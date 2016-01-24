#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    char str[256];
    while(1){
        printf("$ ");
        fgets(str, sizeof(str), stdin);
        str[strlen(str)] = '\0';
        printf("%s\n", str);
    }
    return 0;
}