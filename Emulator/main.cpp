#include <iostream>
#include <string>


#include "Hardware/console.hpp"

using namespace std;

int main(int argc, const char *argv[]) {

	if (argc != 2) {
		cerr << "Wrong use: emulator [nes file]" << endl;
		exit(-1);
	}

	string nesFilename(argv[1]);

	bool isTest = false;
	Hardware::Console nesConsole(nesFilename, isTest);
	nesConsole.run();
	
	return 0;
}
