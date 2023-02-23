/*
*    To run on a Linux system:
*    gcc q1.c
*    ./a.out <insert any number>
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include  <sys/types.h> 
#include <sys/wait.h>

//function to do the calculations 
int Collatz(int num) {

    int cont = 1; //int value to work as a boolean
    while(cont != 0) {  //while loop to continue unil "boolean" value is false (0)
        if(num == 1) { //if num equals 1
            printf("%i ",num);
            cont = 0; //set boolean to false 
            return 0; 
        } else if(num % 2 == 0) { //if num is even
            printf("%i ",num);
            num /= 2; //divide num by 2
        } else { //if num is odd
            printf("%i ",num);
            num = num * 3 + 1; //do n=3n+1
        }
    }
    return 0;
}

//main function
int main(int argc, char *argv[]) {

    int num = 0; //declare variable to store user input
    pid_t pid; //process id variable

    num = atoi(argv[1]); //take user input and store in vairable (string -> int)
    pid = fork(); //create child procces

    if(pid == 0) { //if child process is created
        Collatz(num); //send num to function
    } else if(pid > 0) { // when process id greater than 0
        wait(NULL); //wait
        printf("\n");
    }
}