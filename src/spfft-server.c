#include "spfft-server.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

#define BUFFER_SIZE 20000

struct spffts_iface {
    int sock;
    __uint32_t delay;
};

spffts_iface spffts_openInterface(char *IP, __uint32_t delay){
   	//Create interface
    spffts_iface iface = malloc(sizeof(*iface));
    iface -> sock = nn_socket(AF_SP, NN_REP);
    iface -> delay = delay;
    if(iface -> sock < 0 || nn_bind(iface -> sock, IP) < 0)
		fprintf(stderr, "Error binding socket with IP %s\n", IP);

    return iface;
}

void pollRequests(spffts_iface iface){
    char *message = NULL;
	char buffer[BUFFER_SIZE];
    char test[4];
    int len;
    while(1){
        len = nn_recv(iface -> sock, &message, NN_MSG, 0);
		nn_freemsg(message);

        printf("Message: %s\n", message);
        //TODO: Parse message
        FILE *fp = fopen("toUpload", "r");

        __uint8_t reading = 1;
        size_t total = 0;
        while(reading){
            size_t read = fread(buffer, 1, BUFFER_SIZE, fp);
            total += read;
            if(read != BUFFER_SIZE) reading = 0;
            nn_send(iface -> sock, &read, sizeof(size_t), 0);
			nn_recv(iface -> sock, &message, NN_MSG, 0);
			nn_freemsg(message);
            nn_send(iface -> sock, buffer, BUFFER_SIZE, 0);
			nn_recv(iface -> sock, &message, NN_MSG, 0);
			nn_freemsg(message);
        }
        printf("Read %lu bytes\n", total);
        nn_shutdown(iface -> sock, 0);
        fclose(fp);
        break;
    }
}