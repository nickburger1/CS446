#include <stdio.h>

#define PAGE_NUMBER_MASK 255		
#define OFFSET_MASK 255			
#define PAGES 8
#define OFFSET_BITS  8										
#define PAGE_SIZE 256	

int main(int argc, char *argv[]) {
	unsigned int page_number, frame_number;
	int offset;
	unsigned int virtual_address, physical_address;
	char buff[10];
        int page_table[PAGES] = {6,4,3,7,0,1,2,5};
	if (argc != 2) {
		fprintf(stderr,"Usage: ./a.out <virtual address input file>\n");
		return -1;
	}
	FILE *fptr = fopen(argv[1], "r");

	if (fptr == NULL) {
		printf("Error in opening file \n");
		return -1;
	}
	while(fgets(buff, 10, fptr) != NULL) {
		virtual_address = atoi(buff);
		page_number = (virtual_address >> OFFSET_BITS) & PAGE_NUMBER_MASK;
		frame_number = page_table[page_number];
		offset = virtual_address & OFFSET_MASK;
		physical_address = (frame_number << OFFSET_BITS) | offset;
		printf("Virtual addr is %d: Page# = %d & Offset = %d. Physical addr = %d. \n", virtual_address, page_number, offset, physical_address);
	}
 	fclose(fptr);

	return 0;
}