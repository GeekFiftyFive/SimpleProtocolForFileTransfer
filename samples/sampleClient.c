#include "../src/spfft-client.h"

int main(int argc, char* args[]){
	FILE *testFile = fopen("testFile", "wb");

	spfft_getFile(NULL, "testFile", testFile);

	return fclose(testFile);
}
