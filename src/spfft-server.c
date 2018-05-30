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
    char message[255];
    int len;
    while(1){
        client = SDLNet_TCP_Accept(iface -> sock);
        if(!client) {
            SDL_Delay(iface -> delay);
            continue;
        }

        len = SDLNet_TCP_Recv(client, message, 255);
        printf("Message: %s\n", message);
        //TODO: Parse message
        FILE *fp = fopen("toUpload", "r");

        __uint8_t reading = 1;
        size_t total = 0;
        while(reading){
            size_t read = fread(message + 1, 1, 254, fp);
            total += read;
            if(read < 254) reading = 0;
            message[0] = read;
            SDLNet_TCP_Send(client, message, 255);
        }
        printf("Read %u bytes\n", total);
        SDLNet_TCP_Close(client);
        fclose(fp);
        break;
    }
}