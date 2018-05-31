#include "../src/spfft-client.h"
#include <stdlib.h>
#include <string.h>

char *getCommand(){
    int size = 10, offset = 0;
    int c = EOF;
    char *output = malloc(size);
    
    while((c = getchar()) != '\n' && c != EOF){
        if(offset + 1 == size) {
            size *= 2;
            output = realloc(output, size);
        }
        output[offset] = c;
        offset++;
    }

    return output;
}

int mystrlen(char *p){
    int i = 0;
    while(*(p + i) != '\0') i++;
    return i;
}

char **parseCommand(int *count){
    char *command = getCommand();
    char *token = strtok(command, " \"\n");
    int size = 4, index = 0;
    char **tokens = calloc(size, sizeof(char*));

    while(token){
        tokens[index] = malloc(mystrlen(token) + 1);
        memcpy(tokens[index], token, mystrlen(token) + 1);
        index++;
        token = strtok(NULL, " ");

        if(index == size){
            size *= 2;
            tokens = realloc(tokens, size * sizeof(char*));
        }
    }

    *count = index;
    free(token);
    free(command);

    return tokens;
}

void freeTokens(char **tokens){
}

int main(int argc, char* argv[]){
    //if(argc < 2) return 1;
    //spfftc_iface iface = spfftc_connectInterface(argv[1]);
    //if(!iface) return 2;

    printf("Enter a command, type help for a list of commands\n");

    __uint8_t running = 1;
    while(running){
        printf("> ");
        int count;
        char **tokens = parseCommand(&count);

        for(int i = 0; i < count; i++){
            printf("Token %d: %s\n", i, tokens[i]);
        }

        freeTokens(tokens);
        running = 0;
    }
	
    //FILE *testFile = fopen("testFile", "wb");

	//if(spfftc_getFile(iface, "testFile", testFile)) return 3;

	//if(fclose(testFile)) return 4;
	
	return 0;
}
