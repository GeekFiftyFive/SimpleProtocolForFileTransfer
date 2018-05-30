#ifndef SPFFT_SERVER
#define SPFFT_SERVER

#include <stdio.h>

typedef struct spffts_iface* spffts_iface;

/*
Opens a socket for incoming connections

char *url - Url the server will be accessed via
return    - A pointer to an spfftc_iface struct used to communicate with
            the server
*/
spffts_iface spffts_openInterface(char* url, __uint32_t delay);

/*
Polls requests from clients in an infinite loop

spffts_iface iface - The interface to poll requests on
*/
void pollRequests(spffts_iface iface);

#endif //SPFFT_SERVER