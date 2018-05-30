#include "../src/spfft-client.h"

int main(int argc, char* args[]){
	FILE *testFile = fopen("testFile", "wb");

	if(spfftc_init()) return 1;

	spfftc_iface iface = spfftc_configureInterface("localhost", 8080);

	if(spfftc_getFile(iface, "testFile", testFile)) return 2;

	if(fclose(testFile)) return 3;
	
	return 0;
}
