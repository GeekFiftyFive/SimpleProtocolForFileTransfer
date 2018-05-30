#include "spfft-shared.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int spfft_init(){
	//Init SDL
	if(SDL_Init(0) == -1){
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	//Init SDL_net
	if(SDLNet_Init() == -1){
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		return 2;
	}

	return 0;
}