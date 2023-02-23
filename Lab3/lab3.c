#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

int main() {
    int i = 0;

    printf("Parent Process: %d\n",getpid());
    for(i = 0; i < 8; i++) {
        if(fork() == 0) {
            printf("Child Process: %d\n",getpid());
            exit(0);
        }
        wait(NULL);
    }

    return 0;
}
