#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define MEMORY_SIZE 1024

signed char buffer[MEMORY_SIZE];
signed char *mmapfptr;

int main(int argc, char *argv[]) {
    int mmapfile_fd, i = 0, temp = 0;
    size_t CHAR_SIZE = 1;
    
    if (argc < 2) {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }
    
    mmapfile_fd = open(argv[1], O_RDONLY);
    if (mmapfile_fd == -1) {
        printf("Error opening file %s.\n", argv[1]);
        exit(1);
    }
    
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    if (mmapfptr == MAP_FAILED) {
        printf("Error mapping file %s to memory.\n", argv[1]);
        exit(1);
    }
    
    while (*mmapfptr != '\0') {
        memcpy(buffer + i, mmapfptr + temp, CHAR_SIZE);
        if (*mmapfptr == '\n') {
            temp += 3; // skip the two spaces and the newline character
        } else {
            temp++;
        }
        i++;
    }
    buffer[i] = '\0';
    
    munmap(mmapfptr, MEMORY_SIZE);
    close(mmapfile_fd);
    
    printf("%s", buffer);
    
    return 0;
}
