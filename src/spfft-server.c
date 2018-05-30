#include "spfft-server.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

struct spffts_iface {
    TCPsocket sock;
    __uint32_t delay;
};

spffts_iface spffts_openInterface(__uint16_t port, __uint32_t delay){
    IPaddress addr;
    //Resolve port into IPAddress type
    if(SDLNet_ResolveHost(&addr,NULL,port) == -1){
		fprintf(stderr, "SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		return NULL;
	}

    //Create interface
    spffts_iface iface = malloc(sizeof(*iface));
    iface -> sock = SDLNet_TCP_Open(&addr);
    iface -> delay = delay;
    if(!iface -> sock){
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        free(iface);
        iface = NULL;
    }

    return iface;
}

void pollRequests(spffts_iface iface){
    TCPsocket client;
    char message[1024];
    int len;
    while(1){
        client = SDLNet_TCP_Accept(iface -> sock);
        if(!client) {
            SDL_Delay(iface -> delay);
            continue;
        }

        len = SDLNet_TCP_Recv(client, message, 1024);
        printf("Message: %s\n", message);
        SDLNet_TCP_Close(client);
        break;
    }
}