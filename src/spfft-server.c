#include "spfft-server.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

struct spffts_iface {
    TCPsocket sock;
};

spffts_iface spffts_openInterface(__uint16_t port){
    IPaddress addr;
    //Resolve port into IPAddress type
    if(SDLNet_ResolveHost(&addr,NULL,port) == -1){
		fprintf(stderr, "SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		return NULL;
	}

    //Create interface
    spffts_iface iface = malloc(sizeof(*iface));
    iface -> sock = SDLNet_TCP_Open(&addr);
    if(!iface -> sock){
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        free(iface);
        iface = NULL;
    }

    return iface;
}

void pollRequests(spffts_iface iface){
    
}