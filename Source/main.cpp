#include "System.h"

int main(int argc, char** argv) {
	System* system = System::getInstance();
	system->Run();
	return 0;
}