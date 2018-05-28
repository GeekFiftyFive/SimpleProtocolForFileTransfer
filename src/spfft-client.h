#ifndef SPFFT_CLIENT
#define SPFFT_CLIENT

#include <stdio.h>

typedef struct spfft_iface* spfft_iface;

/*
Initialises SDL and SDL_net, which are used for TCP/IP communication

return      - returns 0 on success
*/
int spfft_init();

/*
Gets a file from the remote server

char *path - The path to the file on the remote server
FILE *fp   - The file to write to on the local machine, must be opened with mode "wb"

return     - returns 0 on success
*/
int spfft_getFile(spfft_iface iface, char *path, FILE *fp);

#endif //SPFFT_CLIENT

