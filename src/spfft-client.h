#ifndef SPFFT_CLIENT
#define SPFFT_CLIENT

#include <stdio.h>

typedef struct spfftc_iface* spfftc_iface;

/*
Connects to a remote spfft server

char *url 		- The url of the remote server

return   - A pointer to an spfftc_iface struct used to communicate with
           the server
*/
spfftc_iface spfftc_connectInterface(char *url);

/*
Gets a file from the remote server

char *path - The path to the file on the remote server
FILE *fp   - The file to write to on the local machine, must be opened with mode "wb"

return     - returns 0 on success
*/
int spfftc_getFile(spfftc_iface iface, char *path, FILE *fp);

/*
Gets a list of files and directories within a directory

char *path - The path on the server to get subdirectories of

return - returns a list of files and directories separated by spaces
*/
char *spfftc_getList(spfftc_iface iface, char *path);

#endif //SPFFT_CLIENT

