#include "spfft-server.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define BUFFER_SIZE 65536

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
    char message[BUFFER_SIZE];
    char test[4];
    int len;
    while(1){
        client = SDLNet_TCP_Accept(iface -> sock);
        if(!client) {
            SDL_Delay(iface -> delay);
            continue;
        }

        len = SDLNet_TCP_Recv(client, message, BUFFER_SIZE);
        printf("Message: %s\n", message);
        //TODO: Parse message
        FILE *fp = fopen("toUpload", "r");

        __uint8_t reading = 1;
        size_t total = 0;
        while(reading){
            size_t read = fread(message, 1, BUFFER_SIZE, fp);
            total += read;
            if(read != BUFFER_SIZE) reading = 0;
            else read = 65536;
            SDLNet_TCP_Send(client, &read, sizeof(size_t));
            SDLNet_TCP_Send(client, message, BUFFER_SIZE);
        }
        printf("Read %lu bytes\n", total);
        SDLNet_TCP_Close(client);
        fclose(fp);
        break;
    }
}