#include <stdio.h>
#include <stdlib.h>


#define PAGE_SIZE 4096      // 4 KB = 2^12
#define OFFSET_BITS 12      // 2^12 = 4096
#define OFFSET_MASK 0xFFF   // 00000000 00000FFF

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <virtual_address>\n", argv[0]);
        return 1;
    }

    unsigned int virtual_address = (unsigned int)atoi(argv[1]);

    unsigned int page_number = virtual_address >> OFFSET_BITS;
    unsigned int offset = virtual_address & OFFSET_MASK;

    printf("The address %u contains:\n", virtual_address);
    printf("Page number = %u\n", page_number);
    printf("Offset = %u\n", offset);

    return 0;
}