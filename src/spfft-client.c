#include "spfft-client.h"
#include <stdlib.h>
#include <time.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <string.h>
#include "spfft-shared.h"

#define BUFFER_SIZE 65536

struct spfftc_iface {
	int sock;
};

spfftc_iface spfftc_connectInterface(char *url){
	//Create an interface
	spfftc_iface iface = malloc(sizeof(*iface));
	iface -> sock = nn_socket(AF_SP, NN_REQ);
	if(iface -> sock < 0 || nn_connect(iface -> sock, url) < 0)
		fprintf(stderr, "Error connecting to socket with IP %s\n", url);

	return iface;
}

int spfftc_getFile(spfftc_iface iface, char *path, FILE *fp){
	char *buffer = NULL;
	char* message = malloc(strlen(path) + 2);
    sprintf(message, "0%s", path); //Create message with opcode '0'
	__uint8_t waiting = 1;
	nn_send(iface -> sock, message, strlen(message) + 1, 0);
    spfft_clientSession *session = NULL;
    nn_recv(iface -> sock, &session, NN_MSG, 0);
    printf("Session{id: %d, secret: %d}\n", session -> id, session -> secret);

    void *blockRequest = malloc(1 + sizeof(spfft_clientSession));
    memcpy(blockRequest, "1", 1);
    memcpy(blockRequest + 1, session, sizeof(spfft_clientSession));

	clock_t begin = clock();
	size_t bytes = 0;
    void *block = NULL;

	while(waiting){
        //Fetch next block
        nn_send(iface -> sock, blockRequest, 1 + sizeof(spfft_clientSession), 0);
        nn_recv(iface -> sock, &block, NN_MSG, 0);

        //Update block counter
        session -> block++;
        memcpy(blockRequest + 1, session, sizeof(spfft_clientSession));

        //Write buffer to disk
        size_t size;
        memcpy(&size, block, sizeof(size_t));
        bytes += size;
        fwrite(block + sizeof(size_t), 1, size, fp);
        nn_freemsg(block);

        //Check if this is the end of the file
        if(size < BUFFER_SIZE) waiting = 0;
	}

	clock_t end = clock();
	float duration = (float)(end - begin) / CLOCKS_PER_SEC;
    free(blockRequest);
    free(message);
    nn_freemsg(session);

	printf("Transferred %lu bytes in %f seconds (%f MB/s)\n", bytes, duration, ((float) bytes / 1000000) / duration);
	nn_shutdown(iface -> sock, 0);
    return 0;
}

char *spfftc_getList(spfftc_iface iface, char *path){
    char *message = malloc(strlen(path) + 2);
    sprintf(message, "2%s", path); //Create a message with opcode 2
    nn_send(iface -> sock, message, (strlen(path) + 2), 0);
    char *list = NULL;
    nn_recv(iface -> sock, &list, NN_MSG, 0);
    return list;
}