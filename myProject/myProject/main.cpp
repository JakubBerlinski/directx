#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	systemClass* system;
	bool result;

	//create the system object
	system = new systemClass;
	if(!system) {
		return 0;
	}

	//initialize and run the system
	result = system->initialize();
	if(result) {
		system->run();
	}

	//shutdown and release system object
	system->shutdown();
	delete system;
	system = 0;

	return 0;
}