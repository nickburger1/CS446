
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMofThreads 2
#define SIZE 10
int list[SIZE] = {12, 19, 28, 67, 91, 53, 2, 8, 29, 14};


void *largestnum(void *param);

typedef struct {
	int from_index;
	int to_index;
	int returnval;
} parameters;

int large1, large2;

int main(int argc, const char *argv[]) {
	int i;
	pthread_t workers[NUMofThreads];
	parameters *data = (parameters *) malloc (sizeof(parameters));
	data->from_index = 0;
	data->to_index = (SIZE/2) - 1;
	data->returnval = 0;
	pthread_create(&workers[0], 0, largestnum, data);
	
	parameters *data1 = (parameters *) malloc (sizeof(parameters));
	data1->from_index = (SIZE/2);
	data1->to_index = SIZE - 1;
	data1->returnval = 0;
	pthread_create(&workers[1], 0, largestnum, data1);
	
	for (i = 0; i < NUMofThreads - 1; i++)
		pthread_join(workers[i], NULL);

		if(data->returnval > data1->returnval){
			printf("The Largest number: %d\n", data->returnval);
		}else {
			printf("The largest number: %d\n", data1->returnval);
		}

    return 0;
}

void *largestnum(void *params)
{
	int i;
	parameters* p = (parameters *)params;
	int begin = p->from_index;
	int end = p->to_index;
	int large = p->returnval;
	
	for (i = begin; i <= end; i++) {
		if (list[i] > large) 
		{
			large = list[i];
		}
	} 
	p->returnval = large;
	pthread_exit(0);
}
