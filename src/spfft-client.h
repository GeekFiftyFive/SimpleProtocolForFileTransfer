#ifndef SPFFT_CLIENT
#define SPFFT_CLIENT

#include <stdio.h>
#include <unistd.h>

typedef struct spfftc_iface* spfftc_iface;

/*
Initialises SDL and SDL_net, which are used for TCP/IP communication

return      - returns 0 on success
*/
int spfftc_init();

/*
Connects to a remote spfft server

char *IP - The IP address of the remote server
int port - The port to connect via

return   - A pointer to an spfftc_iface struct used to communicate with
           the server
*/
spfftc_iface spfftc_configureInterface(char *IP, __uint16_t port);

/*
Gets a file from the remote server

char *path - The path to the file on the remote server
FILE *fp   - The file to write to on the local machine, must be opened with mode "wb"

return     - returns 0 on success
*/
int spfftc_getFile(spfftc_iface iface, char *path, FILE *fp);

#endif //SPFFT_CLIENT

