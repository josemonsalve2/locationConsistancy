#include <stdio.h>
#include <string.h>

void printLoad() {
    printf("LD: \n");
}

void printStore() {
    printf("ST: \n");
}
void printLoadVar(unsigned int i) {
    printf("LD: %u\n", i);
}

void printStoreVar(unsigned int i) {
    printf("ST: %u\n", i);
}
