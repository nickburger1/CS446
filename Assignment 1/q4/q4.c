/*
 * To compile on Linux systems, enter:
 * gcc q4.c -lpthread
 * To run, enter:
 * ./a.out
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// global variables
int unsorted[20] = {10,12,1,18,65,70,75,115,9,24,7,3,54,13,67,43,59,32,10,24};
int sorted[20];
int sublist1[10];
int sublist2[10];
int middleIndex;

// function prototypes
void *runner1(void *param); // this thread will handle sorting the first half
void *runner2(void *param); // this thread will handle sorting the second half
void *runner3(void *param); // this thread will merge the two sorted sublists into the sorted array

int main()
{
    pthread_t tid1, tid2, tid3; // the thread identifiers
    pthread_attr_t attr1, attr2, attr3; // set of attributes for the thread

    /* 
    find the middle point of the array so we can work with 2
    halfs in each thread
    */
    int sizeOfArray = (sizeof(unsorted) / sizeof(int));
    middleIndex = sizeOfArray / 2;

    // get the default attributes
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    //create the first thread
    pthread_create(&tid1,&attr1,runner1,&middleIndex);
    //create the second thread
    pthread_create(&tid2,&attr2,runner2,&middleIndex);

    //now wait for the sorting threads to exit
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    //once the sorting threads exit, we can create the 3rd thread to 
    //merge and sort the sublists
    pthread_create(&tid3,&attr3,runner3,&sizeOfArray);

    //wait for the third thread to finish
    pthread_join(tid3,NULL);

    //print the unsorted array
    printf("Unsorted Array: ");
    for(int i=0; i<20; i++)
    {
        printf("%d ", unsorted[i]);
    }
    printf("\n");

    //print the final sorted array
    printf("Sorted Array: ");
    for(int i=0; i<20; i++)
    {
        printf("%d ", sorted[i]);
    }
    printf("\n");
}

// this thread will handle the first half
void *runner1(void *middleIndex) 
{
    int i, j, a, index = *(int*)middleIndex;
    int temp;
    int swapped;
    
    //copy first half of unsorted array to sublist1
    for(int a = 0; a < index; a++){
        sublist1[a] = unsorted[a];
    }

    //sort sublist1 (bubble sort)
    for (int i = 0; i < index - 1; i++) {
        swapped = 0;
        for (int j = 0; j < index - i - 1; j++) {
            if (sublist1[j] > sublist1[j + 1]) {
                temp = sublist1[j];
                sublist1[j] = sublist1[j + 1];
                sublist1[j + 1] = temp;
                swapped = 1;
            }
        }
        //if swapped never gets set to true(1) throughout entire list, list is now sorted
        if (swapped == 0) {
            break;
        }
    }

    pthread_exit(0);
}

//this thread will handle the second half
void *runner2(void *middleIndex) 
{
    int i, j, a, index = *(int*)middleIndex;
    int temp;
    int swapped;
    
    //copy first half of unsorted array to sublist1
    for(int a = 0; a < 10; a++){
        sublist2[a] = unsorted[a+10];
    }

    //sort sublist2 (bubble sort)
    for (int i = 0; i < index - 1; i++) {
        swapped = 0;
        for (int j = 0; j < index - i - 1; j++) {
            if (sublist2[j] > sublist2[j + 1]) {
                temp = sublist2[j];
                sublist2[j] = sublist2[j + 1];
                sublist2[j + 1] = temp;
                swapped = 1;
            }
        }
        //if swapped never gets set to true(1) throughout entire list, list is now sorted
        if (swapped == 0) {
            break;
        }
    }

    pthread_exit(0);
}

void *runner3(void *sizeOfArray) 
{
    int size = *(int*)sizeOfArray;
    int temp;
    int swapped;

    for(int i = 0; i < 10; i++)
    {
        sorted[i] = sublist1[i];
        sorted[i+10] = sublist2[i];
    }

    for (int i = 0; i < size - 1; i++) {
        swapped = 0;
        for (int j = 0; j < size - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
                swapped = 1;
            }
        }
        //if swapped never gets set to true(1) throughout entire list, list is now sorted
        if (swapped == 0) {
            break;
        }
    }
       
    pthread_exit(0);
}
