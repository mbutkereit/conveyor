#include <cstdlib>
#include <iostream>
#include "Hal/HalBuilder.h"
#include "Thread/ConveyorThread.h"

int main(int argc, char *argv[]) {
	ConveyorThread c1;
	c1.start(NULL);
	c1.join();

	return EXIT_SUCCESS;
}
