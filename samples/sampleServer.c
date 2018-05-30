#include <stdio.h>
#include "../src/spfft-server.h"
#include "../src/spfft-shared.h"

int main(int argc, char* args[]){
    if(spfft_init()) return 1;
    spffts_iface iface = spffts_openInterface("tcp://192.168.0.11:8080", 100);
    pollRequests(iface);
    return 0;
}