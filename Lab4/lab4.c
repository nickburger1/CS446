#include <stdio.h>
#include <pthread.h>
#define MAX 20
#define MAX_THREAD 2
int list[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int add[2] = { 0 };
int part = 0;

void *largestnum(void *params) {
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

int main() {
	pthread_t threads[MAX_THREAD];
	for (int i = 0; i < MAX_THREAD; i++){ 
		pthread_create(&threads[i], NULL, largestnum, (void*)NULL);
	}

	for (int i = 0; i < MAX_THREAD; i++) {
		pthread_join(threads[i], NULL);
	}

	int total = 0;
	for (int i = 0; i < MAX_THREAD; i++){
		total+= add[i];
	}
		
	printf("\nSum is %d\n", total );
	return 0;
}
