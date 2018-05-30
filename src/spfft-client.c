#include "spfft-client.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <string.h>

#define BUFFER_SIZE 255

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

	while(waiting){
		SDLNet_TCP_Recv(iface -> sock, buffer, BUFFER_SIZE);
		if(buffer[0] < BUFFER_SIZE - 1) waiting = 0;
		printf("Bytes in message: %d\n", buffer[0]);
		fwrite(buffer + 1, 1, buffer[0], fp);
	}
	
    return 0;
}