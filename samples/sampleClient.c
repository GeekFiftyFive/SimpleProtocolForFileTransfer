#include "../src/spfft-client.h"

int main(int argc, char* args[]){
	FILE *testFile = fopen("testFile", "wb");

	spfftc_iface iface = spfftc_connectInterface("tcp://192.168.0.11:8080");

	if(!iface) return 2;

	if(spfftc_getFile(iface, "testFile", testFile)) return 3;

	if(fclose(testFile)) return 4;
	
	return 0;
}
