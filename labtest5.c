#include <sys/mman.h> 
#include <sys/stat.h>
#include <string.h> 
#include <fcntl.h>
#include <stdlib.h> 
#include <stdio.h> 

signed char* mmapfptr;
signed char* mmapfptr_2;
struct stat sb;
struct stat sb_2;


int main(int argc, char* argv[]) {
	
	int mmapfile_fd = open(argv[1], O_RDONLY);
	fstat(mmapfile_fd, &sb);
	signed char file1buffer[sb.st_size];
	mmapfptr = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);

	int i = 0;
	int temp = 0;
	for (i = 0; i < sb.st_size; i++) {
		memcpy(file1buffer + i, mmapfptr + temp, 1);
		if (i % 1 == 0) {
			temp += 3;
		}
		else {
			temp += 2;
		}
	}

	munmap(mmapfptr, sb.st_size);
    mmapfile_fd = open(argv[2], O_RDONLY);
	fstat(mmapfile_fd, &sb);
    signed char file2buffer[sb.st_size];
	mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);

	int j = 0;
	temp = 0;
	for (j = 0; j < sb.st_size; j++) {
		memcpy(file2buffer + j, mmapfptr + temp, 1);
		if (j % 1 == 0) {
			temp += 2;
		}
		else {
			temp += 3;
		}
	}
	munmap(mmapfptr, sb.st_size);
	printf("%s %s\n", file1buffer, file2buffer);
	return 0;
}