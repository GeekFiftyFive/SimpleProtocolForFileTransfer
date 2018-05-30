#include "spfft-client.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

struct spfftc_iface {
	TCPsocket sock;
};

int spfftc_init(){
	//Init SDL
	if(SDL_Init(0) == -1){
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	//Init SDL_net
	if(SDLNet_Init() == -1){
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		return 2;
	}

	return 0;
}

spfftc_iface spfftc_configureInterface(char *IP, __uint16_t port){
	printf("Attempting connection to %s:%d\n", IP, port);
	IPaddress addr;
	//Resolve server details into IPaddress type
	if(SDLNet_ResolveHost(&addr, IP, port) == -1){
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return NULL;
	}

	//Create an interface
	spfftc_iface iface = malloc(sizeof(*iface));
	iface -> sock = SDLNet_TCP_Open(&addr);
	if(!iface -> sock){
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		free(iface);
		iface = NULL;
	}

	return iface;
}

int spfftc_getFile(spfftc_iface iface, char *path, FILE *fp){
    return 0;
}