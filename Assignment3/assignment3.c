#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int PAGE_TABLE_SIZE = 256;
const int BUFFER_SIZE = 256;
const int PHYS_MEM_SIZE = 256;
const int TLB_SIZE = 16;

struct TLB {
	unsigned char TLBpage[16];
	unsigned char TLBframe[16];
	int ind;
};
	
int readFromDisk (int pageNum, char *PM, int* OF){

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));

	FILE *BS;
	BS = fopen("BACKING_STORE.bin", "rb");
	
	if (fseek(BS, pageNum * PHYS_MEM_SIZE, SEEK_SET)!=0)
		printf("fseek Error\n");

	if (fread(buffer, sizeof(char), PHYS_MEM_SIZE, BS)==0)
		printf("fread Error\n");

	int i = 0;
	for(i; i < PHYS_MEM_SIZE; i++){
		*((PM+(*OF)*PHYS_MEM_SIZE)+i) = buffer[i];
	}
	
	(*OF)++;
	return (*OF)-1;
} 

int findPage(int logicalAddr, char* PT, struct TLB *tlb,  char* PM, int* OF, int* pageFaults, int* TLBhits){

	unsigned char mask = 0xFF;
	unsigned char offset;
	unsigned char pageNum;
	bool TLBhit = false;
	int frame = 0;
	int value;
	int newFrame = 0;

	printf("Logical adress: %d\t", logicalAddr);
	pageNum = (logicalAddr >> 8) & mask;
	offset = logicalAddr & mask;

	int i = 0;
	for (i; i < TLB_SIZE; i++){
		if(tlb->TLBpage[i] == pageNum){
			frame = tlb->TLBframe[i];
			TLBhit = true;
			(*TLBhits)++;	
		}		
	}

	if (TLBhit == false){
		if (PT[pageNum] == -1){
            newFrame = readFromDisk(pageNum, PM, OF);
			PT[pageNum] = newFrame;
			(*pageFaults)++;
		}

		frame = PT[pageNum];
		tlb->TLBpage[tlb->ind] = pageNum;
		tlb->TLBframe[tlb->ind] = PT[pageNum];
		tlb->ind = (tlb->ind + 1)%TLB_SIZE;	
	}

	int index = ((unsigned char)frame*PHYS_MEM_SIZE)+offset;
	value = *(PM+index);
	printf("Physical address: %d\t Value: %d\n",index, value);	
	return 0;
}

int main (int argc, char* argv[]){
	
	int val;
	FILE *fd;
	int openFrame = 0;
	int pageFaults = 0;
	int TLBhits = 0;
	int inputCount = 0;
	float pageFaultRate;
	float TLBHitRate;
	unsigned char PageTable[PAGE_TABLE_SIZE];

	memset(PageTable, -1, sizeof(PageTable));	
	struct TLB tlb;	

	memset(tlb.TLBpage, -1, sizeof(tlb.TLBpage));
	memset(tlb.TLBframe, -1, sizeof(tlb.TLBframe));

	tlb.ind = 0;
	char PhyMem[PHYS_MEM_SIZE][PHYS_MEM_SIZE]; 
	fd = fopen(argv[1], "r");
	
	while (fscanf(fd, "%d", &val)==1){
		findPage(val, PageTable, &tlb, (char*)PhyMem, &openFrame, &pageFaults, &TLBhits);
		inputCount++;
	}

	printf("\nNumber of Translated Addresses = %d\nPage Faults = %d\nTLB Hits= %d\n",inputCount, pageFaults, TLBhits);
	return 0;

}