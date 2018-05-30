#ifndef SPFFT_SERVER
#define SPFFT_SERVER

#include <stdio.h>

typedef struct spffts_iface* spffts_iface;

/*
Initialises SDL and SDL_net, which are used for TCP/IP communication

return      - returns 0 on success
*/
int spffts_init();

/*
Opens a socket for incoming connections

__uint16_t port - The port to connect via

return   - A pointer to an spfftc_iface struct used to communicate with
           the server
*/
spffts_iface spffts_openInterface(__uint16_t port);

#endif //SPFFT_SERVER