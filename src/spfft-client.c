#include "spfft-client.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>

#define BUFFER_SIZE 65536

struct spfftc_iface {
	TCPsocket sock;
};

spfftc_iface spfftc_connectInterface(char *IP, __uint16_t port){
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
	unsigned char buffer[BUFFER_SIZE];
	const char* message = "0 testFile";
	__uint8_t waiting = 1;
	SDLNet_TCP_Send(iface -> sock, message, strlen(message) + 1);

	Uint32 start = SDL_GetTicks();
	size_t bytes = 0;

	while(waiting){
        size_t len;
        SDLNet_TCP_Recv(iface -> sock, &len, sizeof(size_t));
		SDLNet_TCP_Recv(iface -> sock, buffer, BUFFER_SIZE);
		if(len < BUFFER_SIZE) waiting = 0;
        if(len > BUFFER_SIZE) printf("len: %lu\n", len);
		bytes += len;
		fwrite(buffer, 1, len, fp);
	}

	Uint32 end = SDL_GetTicks();
	end -= start;
	float duration = end / 1000;
	printf("Transferred %lu bytes in %f seconds (%f MiB/s)\n", bytes, duration, ((float) bytes / 1000000) / duration);
	
    return 0;
}