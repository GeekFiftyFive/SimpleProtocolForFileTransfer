#include <stdio.h>
#include "../src/spfft-server.h"

int main(int argc, char* args[]){
    spffts_iface iface = spffts_openInterface("tcp://192.168.0.11:8080", 100);
    pollRequests(iface);
    return 0;
}