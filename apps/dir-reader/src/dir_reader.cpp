#include "directory_stream.hpp"

int main(int argc, char* argv[]) {
	
	//displayMessage("Launching Directory Reader Demo App!", MESSAGE_NORMAL, __FUNCTION__);

	bool wantsToAutoscale = true;

	directoryManager dM;

	if (!dM.initializeInput(argc, argv)) return -1;
	dM.initializeOutput(argc, argv);
	dM.initialize();
	dM.setWriteMode(!(argc >= 4));
	dM.setLoopMode(true);
	dM.set_autoscaleTemps(wantsToAutoscale);
	
	while (dM.wantsToRun()) {
		dM.grabFrame();
		dM.processFrame();
		dM.colorizeFrame();
		dM.displayFrame();
		dM.writeImageToDisk();
	}
	
	return S_OK;
}
