#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

//global array and mutex
int *fibNumbers;
pthread_mutex_t fibMutex = PTHREAD_MUTEX_INITIALIZER;

//function prototypes
void *runner1(void *param); // this thread will handle generating the fibonacci numbers

int main(int argc, char *argv[]) {
    //Error check number of command line parameters
    if (argc != 2) {
        printf("Usage: ./a.out <number of Fibonacci numbers>\n");
        return 1;
    }

    //handle user input
    int numberOfFib = atoi(argv[1]);
    if(numberOfFib <= 0){
        printf("Must enter a 0 or positive integer");

    }

    //dynamically allocate fib array's size
    fibNumbers = (int*)malloc(numberOfFib * sizeof(int));
    if (fibNumbers == NULL) 
    {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    pthread_t tid1; //thread identifier
    pthread_attr_t attr1; //attribute for the thread
    pthread_attr_init(&attr1); //get the default attributes

    //create the thread and pass in the number of fibonacci numbers to be generated
    pthread_create(&tid1,&attr1,runner1,argv[1]);

    // now wait for the thread to exit
    pthread_join(tid1,NULL);

    printf("Number of Fibonacci Numbers = %d\n",numberOfFib);

    //once thread is finished, print out all the numbers
    for(int i=0; i<numberOfFib; i++)
    {
        printf("%d ", fibNumbers[i]);
    }

    free(fibNumbers);
    return 0;
}

// this thread will handle the fibonacci numbers generation
void *runner1(void *param) 
{
    int i, n = atoi(param);
    
    //first two numbers in fibonacci are always 0 and 1
    fibNumbers[0] = 0;
    fibNumbers[1] = 1;

    //loop from 0 to middle index
    for (i = 2; i <= n; i++)
    {
        //protect critical section with mutex
        pthread_mutex_lock(&fibMutex);
        fibNumbers[i] = fibNumbers[i-1] + fibNumbers[i-2];
        pthread_mutex_unlock(&fibMutex);
    }
        
    pthread_exit(0);
} 
