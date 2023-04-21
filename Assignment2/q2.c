#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUMBER_OF_DARTS 50000000
#define NUMBER_OF_THREADS 4

/* the number of hits in the circle */
int circle_count = 0;

/* mutex lock */
pthread_mutex_t lock;

/*
 * Generates a double precision random number
 */
double random_double() {
    return random() / ((double)RAND_MAX + 1);
}

void *worker(void *param) {
    int number_of_darts;
    number_of_darts = *((int *)param);
    int i;
    int hit_count = 0;
    double x, y;
    double squareRoot;
    for (i = 0; i < number_of_darts; i++) {
        /* generate random numbers between -1.0 and +1.0 (exclusive) */
        x = random_double() * 2.0 - 1.0;
        y = random_double() * 2.0 - 1.0;
        squareRoot = sqrt(x*x + y*y);
        if (squareRoot < 1.0) {
            ++hit_count;
        }
    }

    // lock mutex before updating the shared variable
    pthread_mutex_lock(&lock);
    circle_count += hit_count;
    pthread_mutex_unlock(&lock);

    pthread_exit(0);
}

int main(int argc, const char * argv[]) {
    int darts_per_thread = NUMBER_OF_DARTS / NUMBER_OF_THREADS;
    int i;
    double estimated_pi;
    pthread_t workers[NUMBER_OF_THREADS];

    /* seed the random number generator */
    srandom((unsigned)time(NULL));

    // initialize the mutex lock
    pthread_mutex_init(&lock, NULL);

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_create(&workers[i], 0, worker, &darts_per_thread);
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(workers[i],NULL);
    }

    // destroy the mutex lock
    pthread_mutex_destroy(&lock);

    /* estimate Pi */
    estimated_pi = 4.0 * circle_count / NUMBER_OF_DARTS;
    printf("Pi = %f\n", estimated_pi);

    return 0;
}
