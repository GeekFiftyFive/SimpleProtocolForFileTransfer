#ifndef __SPFFT_SHARED
#define __SPFFT_SHARED

#include <stdlib.h>

typedef struct spfft_clientSession{
    __uint32_t id;
    int secret;
    __uint32_t block;
    __uint32_t index;
} spfft_clientSession;

#endif //__SPFFT_SHARED