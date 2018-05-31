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
	const char* message = "0testFile";
	__uint8_t waiting = 1;
	nn_send(iface -> sock, message, strlen(message) + 1, 0);
    spfft_clientSession *session = NULL;
    nn_recv(iface -> sock, &session, NN_MSG, 0);
    printf("Session{id: %d, secret: %d}\n", session -> id, session -> secret);

    void *blockSizeRequest = malloc(1 + sizeof(spfft_clientSession));
    memcpy(blockSizeRequest, "1", 1);
    memcpy(blockSizeRequest + 1, session, sizeof(spfft_clientSession));

	clock_t begin = clock();
	size_t bytes = 0;
    void *block = malloc(BUFFER_SIZE + sizeof(size_t));

	while(waiting){
        nn_send(iface -> sock, blockSizeRequest, 1 + sizeof(spfft_clientSession), 0);
        nn_recv(iface -> sock, &block, NN_MSG, 0);
        size_t size;
        memcpy(&size, block, sizeof(size_t));
        bytes += size;
        fwrite(block + sizeof(size_t), 1, size, fp);
        if(size < BUFFER_SIZE) waiting = 0;
	}

	clock_t end = clock();
	float duration = (float)(end - begin) / CLOCKS_PER_SEC;
    free(blockSizeRequest);
    //free(block);
	printf("Transferred %lu bytes in %f seconds (%f MB/s)\n", bytes, duration, ((float) bytes / 1000000) / duration);
	nn_shutdown(iface -> sock, 0);
    return 0;
}