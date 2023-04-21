#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define DepositThreads 7
#define WithdrawThreads 3
#define true 1
#define LIMIT 400

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
void *deposit(void *param);
void *withdraw(void *param);

int balance=0;

void *deposit(void *param) {
	printf("Depositing...\n");
	while (true) { 
		int v = atoi(param);
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);				
		balance = balance + v;
		printf("Current Balance = %d \n", balance);	
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		pthread_exit(0);
	}
}

void *withdraw(void *param) {
	printf("Withdrawing...\n");
	while (true) { 
		int v = atoi(param);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		balance = balance - v;	
		printf("Current Balance = %d \n", balance);	
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
		pthread_exit(0);
	}
}

int main(int argc, char *argv[]) {
	int i, j, max;
	
	pthread_t deposittid[DepositThreads];
	pthread_t withdrawtid[WithdrawThreads];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	if (argc !=3){
		printf("usage: a.out <deposit integer value> <withdraw integer value> \n");
		return -1;	
	}
	if (atoi(argv[1]) < 0){
		printf("%d must be >=0 \n", atoi(argv[1]));	
	}
	if (atoi(argv[2]) < 0){
		printf("%d must be >=0 \n", atoi(argv[2]));	
	}
	if (pthread_mutex_init(&mutex, NULL) !=0){
		printf("Error in initializing mutex \n");	
	}
	if (sem_init(&full, 0, 0) !=0){
		printf("Error in initializing semaphore full \n");	
	}
	
	max = LIMIT/100;
	
	if (sem_init(&empty, 0, max) !=0){
		printf("Error in initializing semaphore full \n");	
	}
	for(i = 0; i < DepositThreads; i++) {
		if(pthread_create(&deposittid[i], &attr, deposit, argv[1])!=0)
			printf("Error in Deposit Thread %d \n", i);
	}
	for(i = 0; i < WithdrawThreads; i++) {
		if(pthread_create(&withdrawtid[i], &attr, withdraw, argv[2])!=0){
			printf("Error in Withdraw Thread %d \n", i);
		}	
	}
	for(i = 0; i < DepositThreads; i++) {
		pthread_join(deposittid[i], NULL);		
	}
	for(i = 0; i < WithdrawThreads; i++) {
		pthread_join(withdrawtid[i], NULL);	
	}

	printf("Final Balance = %d \n", balance);	
	return 0;
}

