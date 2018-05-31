#include "spfft-server.h"
#include <stdlib.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <time.h>
#include "spfft-shared.h"

#define BUFFER_SIZE 65536

typedef struct session{
	FILE *fp;
	int secret;
	float progress;
} session;

struct spffts_iface {
    int sock;
    __uint32_t delay;
	session *sessions;
    __uint32_t lastIndex;
    __uint32_t size;
};

spffts_iface spffts_openInterface(char *url, __uint32_t delay){
   	//Create interface
    spffts_iface iface = malloc(sizeof(*iface));
    iface -> sock = nn_socket(AF_SP, NN_REP);
    iface -> delay = delay;
    iface -> size = 1024;
    iface -> sessions = malloc(sizeof(struct session) * iface -> size);
    
    //Seed the random number generator for shared secrets
    //TODO: Use a cryptographically random number
    time_t t;
    srand((unsigned) time(&t));

    //Initialise the file pointers for each session to NULL
    for(int i = 0; i < iface -> size; i++) iface -> sessions[i].fp = NULL;
    iface -> lastIndex = 0;
    if(iface -> sock < 0 || nn_bind(iface -> sock, url) < 0)
		fprintf(stderr, "Error binding socket with IP %s\n", url);

    return iface;
}

void spffts_closeInterface(spffts_iface iface){
    free(iface -> sessions);
    nn_shutdown(iface -> sock, 0);
    free(iface);
}

void getBlock(spffts_iface iface, char* message){
    spfft_clientSession session;
    memcpy(&session, message + 1, sizeof(spfft_clientSession));
    __uint32_t id = session.id;
    if(session.secret != iface -> sessions[id].secret){
        nn_send(iface -> sock, "inv_secret", 11, 0);
        return;
    }

    void *block = malloc(BUFFER_SIZE + sizeof(size_t));
    size_t read = fread(block + sizeof(size_t), 1, BUFFER_SIZE, iface -> sessions[id].fp);
    memcpy(block, &read, sizeof(size_t));
    if(read < BUFFER_SIZE) {
        fclose(iface -> sessions[id].fp);
        iface -> sessions[id].fp = NULL; //Make session slot available again
        iface -> lastIndex = id;
    }
    nn_send(iface -> sock, block, sizeof(size_t) + BUFFER_SIZE, 0);
    free(block);
}

void startSession(spffts_iface iface, char *message){
    while(iface -> sessions[iface -> lastIndex].fp){
        if(iface -> lastIndex > iface -> size){
            iface -> size *= 2;
            realloc(iface -> sessions, iface -> size);
        }
        iface -> lastIndex++;
    }
    
    __uint32_t id = iface -> lastIndex;
    iface -> sessions[id].fp = fopen(message + 1, "r");
    iface -> sessions[id].secret = rand();
    iface -> sessions[id].progress = 0.f;
    spfft_clientSession session = {id, iface -> sessions[id].secret};
    nn_send(iface -> sock, &session, sizeof(spfft_clientSession), 0);
    printf("ID: %d, Secret: %d\n", id, iface -> sessions[id].secret);
}

void pollRequests(spffts_iface iface){
    char *message = NULL;
	char buffer[BUFFER_SIZE];
    char test[4];
    int len;
    while(1){
        len = nn_recv(iface -> sock, &message, NN_MSG, 0);

        switch(message[0]){
            case '0': //Begin transfer
                printf("Beginning transfer\n");
                startSession(iface, message);
                break;
            case '1': //Get next block
                getBlock(iface, message);
                break;
            default:
                break;
        }
        nn_freemsg(message);
        /*FILE *fp = fopen("toUpload", "r");

        __uint8_t reading = 1;
        size_t total = 0;
		//TODO: Handle more than one user
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
        printf("Read %lu bytes\n", total);*/
        //if(message[0] != '1') continue;
        //nn_shutdown(iface -> sock, 0);
        //break;
        //fclose(fp);
    }
}