#include <stdio.h>
#include "../src/spfft-server.h"
#include "../src/spfft-shared.h"

int main(int argc, char* args[]){
    printf("Hello World!\n");
    if(spfft_init()) return 1;
    return 0;
}