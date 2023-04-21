#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SHM_NAME "/collatz" //define shared memory name

int main() {

    //open shared memory object and check for error
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // map shared memory object to process address space and check for error
    int *sequence = mmap(NULL, sizeof(int) * 1000, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (sequence == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    //print sequence from shared memory object 
    int i = 0;
    while (sequence[i] != -1) {
        printf("%d ", sequence[i]);
        i++;
    }
    printf("\n");

    // detach shared memory object from process address space and check for error
    if (munmap(sequence, sizeof(int) * 1000) == -1) {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    //unlink shared memory object and check for error
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink failed");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
