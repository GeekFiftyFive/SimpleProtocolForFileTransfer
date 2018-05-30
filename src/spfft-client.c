#include "spfft-client.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <string.h>

#define BUFFER_SIZE 20000

struct spfftc_iface {
	int sock;
};

spfftc_iface spfftc_connectInterface(char *IP, __uint16_t port){
	//Create an interface
	spfftc_iface iface = malloc(sizeof(*iface));
	iface -> sock = nn_socket(AF_SP, NN_REQ);
	if(iface -> sock < 0 || nn_connect(iface -> sock, IP) < 0)
		fprintf(stderr, "Error connecting to socket with IP %s\n", IP);

	return iface;
}

int spfftc_getFile(spfftc_iface iface, char *path, FILE *fp){
	char *buffer = NULL;
	const char* message = "0 testFile";
	__uint8_t waiting = 1;
	nn_send(iface -> sock, message, strlen(message) + 1, 0);

	Uint32 start = SDL_GetTicks();
	size_t bytes = 0;

	while(waiting){
    	size_t *len = NULL;
        nn_recv(iface -> sock, &len, NN_MSG, 0);
		nn_send(iface -> sock, "GOT", 4, 0);
		nn_recv(iface -> sock, &buffer, NN_MSG, 0);
		nn_send(iface -> sock, "GOT", 4, 0);

		if(*len < BUFFER_SIZE) waiting = 0;
        if(*len > BUFFER_SIZE) printf("len: %lu\n", *len);
		bytes += *len;
		fwrite(buffer, 1, *len, fp);
		nn_freemsg(buffer);
		nn_freemsg(len);
	}

	Uint32 end = SDL_GetTicks();
	end -= start;
	float duration = end / 1000;
	printf("Transferred %lu bytes in %f seconds (%f MiB/s)\n", bytes, duration, ((float) bytes / 1000000) / duration);
	nn_shutdown(iface -> sock, 0);
    return 0;
}