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

    if(offset == size) output = realloc(output, size + 1);
    output[offset] = '\0';

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

void freeTokens(char **tokens, int count){
    for(int i = 0; i < count; i++) free(tokens[i]);
    free(tokens);
}

int main(int argc, char* argv[]){
    if(argc < 2) return 1;
    spfftc_iface iface = spfftc_connectInterface(argv[1]);
    if(!iface) return 2;

    printf("Enter a command, type help for a list of commands\n");

    __uint8_t running = 1;

    while(running){
        printf("> ");
        int count;
        char **tokens = parseCommand(&count);

        if(strcmp(tokens[0], "get") == 0 && count == 2){
            FILE *testFile = fopen(tokens[1], "wb");

	        if(spfftc_getFile(iface, tokens[1], testFile)) fprintf(stderr, "Error getting file\n");

	        if(fclose(testFile)) fprintf(stderr, "Error closing file\n");
        } else if(strcmp(tokens[0], "exit") == 0) running = 0;
        else if(strcmp(tokens[0], "ls") == 0 && count == 2) 
            printf("%s\n", spfftc_getList(iface, tokens[1]));
        else printf("Error: Invalid command, %s\n", tokens[0]);

        freeTokens(tokens, count);
    }
	
	return 0;
}
