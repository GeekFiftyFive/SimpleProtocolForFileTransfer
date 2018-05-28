#ifndef SPFFT_CLIENT
#define SPFFT_CLIENT

#include <stdio.h>

/*
Gets a file from the remote server

char *path - The path to the file on the remote server
FILE *fp   - The file to write to on the local machine, must be opened with mode "wb"
*/
int spfft_getFile(char *path, FILE *fp);

#endif //SPFFT_CLIENT

