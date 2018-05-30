#include "spfft-client.h"
#include <stdlib.h>
#include <time.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <string.h>

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
	const char* message = "0 testFile";
	__uint8_t waiting = 1;
	nn_send(iface -> sock, message, strlen(message) + 1, 0);

	clock_t begin = clock();
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

	clock_t end = clock();
	float duration = (float)(end - begin) / CLOCKS_PER_SEC;
	printf("Transferred %lu bytes in %f seconds (%f MiB/s)\n", bytes, duration, ((float) bytes / 1000000) / duration);
	nn_shutdown(iface -> sock, 0);
    return 0;
}