#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_NAME "/collatz" //define shared memory name

int main(int argc, char *argv[]) {

    int start_num = atoi(argv[1]); //get user input
   
    // create shared memory object and check for error
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // set the size of the shared memory object and check for error
    if (ftruncate(shm_fd, sizeof(int) * 1000) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    // map shared memory object to process address space and check for error
    int *sequence = mmap(NULL, sizeof(int) * 1000, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sequence == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    //generate the collatz sequence for the given input
    int i = 0;
    while (start_num != 1) {
        sequence[i] = start_num;
        i++;
        if (start_num % 2 == 0) {
            start_num /= 2;
        } else {
            start_num = 3 * start_num + 1;
        }
    }
    start_num = 1;
    sequence[i] = start_num;
    sequence[i+1] = -1;
    
    // detach shared memory object from process address space and check for error
    if (munmap(sequence, sizeof(int) * 1000) == -1) {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
