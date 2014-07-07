#include "input_stream.hpp"

streamerSharedData::streamerSharedData() : 
	detectorMode(DETECTOR_MODE_INS),
	usbMode(USB_MODE_16),
	inputDatatype(DATATYPE_RAW),
	maxReadAttempts(0), 
	framerate(-1.0),
	normMode(NORM_MODE_STANDARD),
	normFactor(0.0),
	output16bit(false), 
	output8bit(true),
	outputColor(false),
	map(CONFIG_MAP_CODE_CIELUV),
	undistortImages(false), 
	fusionFactor(0.6),  
	serialPollingRate(25.0),
	maxNucInterval(45),
	maxNucThreshold(0.2),
	verboseMode(false), 
	autoTemperature(false),
	minTemperature(25.0), 
	maxTemperature(35.0),
	tempGrad(100.0),
	tempIntercept(0.0),
	debugMode(false)
{ }
	
streamerData::streamerData() : 
	filterMode(IMAGE_FILTER_NONE), 
	filterParam(2.0),
	outputType(OUTPUT_TYPE_CV_16UC1),
	syncMode(SYNCMODE_HARD), 
	camera_number(0), 
	desiredRows(-1), 
	desiredCols(-1), 
	temporalMemory(5), 
	radiometryFile("radiometryFile"), 
	externalNucManagement(""), 
	portAddress("/dev/ttyUSB0"), 
	source("dev"), 
	filename("file"), 
	capture_device( "/dev/video0"), 
	folder("folder"), 
	intrinsics("intrinsics"), 
	extrinsics("extrinsics"), 
	addExtrinsics(false),
	imageDimensionsSpecified(false),
	topicname("/thermalvis/streamer/image_raw"), 
	normalizationMode("normalizationMode"), 
	intrinsicsProvided(false), 
	timeStampsAddress(""), 
	republishTopic("specifyTopic/image_raw"), 
	outputFolder("outputFolder"), 
	frameID(""), 
	outputTimeFile(""), 
	outputVideo("outputVideo"), 
	videoType("videoType"), 
	outputTypeString(""), 
	radiometricCorrection(true), 
	radiometricRaw(false), 
	serialFeedback(false), 
	useCurrentRosTime(false), 
	alreadyCorrected(true), 
	markDuplicates(false), 
	outputDuplicates(false), 
	smoothThermistor(false), 
	radiometricInterpolation(true), 
	displayThermistor(false), 
	serialComms(false), 
	readThermistor(true), 
	forceInputGray(false), 
	fixDudPixels(true), 
	disableSkimming(true), 
	readMode(false), 
	loadMode(false), 
	captureMode(false), 
	pollMode(false), 
	subscribeMode(false), 
	resampleMode(false), 
	loopMode(false), 
	resizeImages(false), 
	dumpTimestamps(false), 
	removeDuplicates(false), 
	temporalSmoothing(false), 
	pauseMode(false), 
	extremes(true), 
	stepChangeTempScale(false), 
	rectifyImages(false), 
	writeImages(false), 
	keepOriginalNames(false), 
	writeVideo(false), 
	republishNewTimeStamp(false), 
	drawReticle(false), 
	autoAlpha(true), 
	radiometricBias(0), 
	calibrationMode(CALIBMODE_OFF), 
	alternatePeriod(5), 
	inputWidth(0), 
	inputHeight(0), 
	serialCommsConfigurationCode(SERIAL_COMMS_CONFIG_DEFAULT), 
	serialWriteAttempts(1), 
	republishSource(NO_REPUBLISH_CODE), 
	outputFormat(4), 
	thermistorWindow(5.0), 
	syncDiff(0.005), 
	writeQuality(1.0), 
	maxThermistorDiff(0.5),
	maxIntensityChange(2.0), 
	alpha(0.00),
	dataValid(true)
{ }

bool streamerData::assignFromXml(xmlParameters& xP) {

	int countOfNodes = 0;

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, xP.pt.get_child("launch")) {
		if (v.first.compare("node")) continue; // only progresses if its a "node" tag
		if (!v.second.get_child("<xmlattr>.type").data().compare("streamer")) countOfNodes++;
	}

	if (countOfNodes == 0) {
		ROS_ERROR("No streamer nodes found in XML config!");
		return false;
	}

	if (countOfNodes > 1) {
		ROS_ERROR("More than 1 node of same type found in XML config! This functionality is not supported in Windows..");
		return false;
	}

	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, xP.pt.get_child("launch")) { // Within tree (pt), finds launch, and loops all tags within it
		if (v.first.compare("node")) continue;
		if (v.second.get_child("<xmlattr>.type").data().compare("streamer")) continue;

		BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second) { // Traverses the subtree...
			if (v2.first.compare("param")) continue;

			// From <streamerSharedData>

			if (!v2.second.get_child("<xmlattr>.name").data().compare("verboseMode")) verboseMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("debugMode")) debugMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("output16bit")) output16bit = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("output8bit")) output8bit = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputColor")) outputColor = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("undistortImages")) undistortImages = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("autoTemperature")) autoTemperature = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			
			if (!v2.second.get_child("<xmlattr>.name").data().compare("maxReadAttempts")) maxReadAttempts = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("normMode")) normMode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("maxNucInterval")) maxNucInterval = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("map")) map = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("inputDatatype")) inputDatatype = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("detectorMode")) detectorMode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("usbMode")) usbMode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			
			if (!v2.second.get_child("<xmlattr>.name").data().compare("framerate")) framerate = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("normFactor")) normFactor = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("fusionFactor")) fusionFactor = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("serialPollingRate")) serialPollingRate = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("maxNucThreshold")) maxNucThreshold = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("minTemperature")) minTemperature = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("maxTemperature")) maxTemperature = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("tempGrad")) tempGrad = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("tempIntercept")) tempIntercept = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			
			// From <streamerData>

			if (!v2.second.get_child("<xmlattr>.name").data().compare("syncMode")) syncMode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("camera_number")) camera_number = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("desiredRows")) desiredRows = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("desiredCols")) desiredCols = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("mapCode")) mapCode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("mapParam")) mapParam = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("temporalMemory")) temporalMemory = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputType")) outputType = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());

			if (!v2.second.get_child("<xmlattr>.name").data().compare("radiometryFile")) radiometryFile = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("externalNucManagement")) externalNucManagement = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("portAddress")) portAddress = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("read_addr")) read_addr = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("source")) source = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("filename")) filename = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("folder")) folder = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("directory")) folder = v2.second.get_child("<xmlattr>.value").data();

			if (!v2.second.get_child("<xmlattr>.name").data().compare("capture_device")) capture_device = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("intrinsics")) intrinsics = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("extrinsics")) extrinsics = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("topicname")) topicname = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("normalizationMode")) normalizationMode = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("timeStampsAddress")) timeStampsAddress = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("republishTopic")) republishTopic = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputFolder")) outputFolder = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("frameID")) frameID = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputFormatString")) outputFormatString = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputTimeFile")) outputTimeFile = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputVideo")) outputVideo = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("videoType")) videoType = v2.second.get_child("<xmlattr>.value").data();
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputTypeString")) outputTypeString = v2.second.get_child("<xmlattr>.value").data();
			
			if (!v2.second.get_child("<xmlattr>.name").data().compare("radiometricCorrection")) radiometricCorrection = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("radiometricRaw")) radiometricRaw = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("serialFeedback")) serialFeedback = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("useCurrentRosTime")) useCurrentRosTime = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("alreadyCorrected")) alreadyCorrected = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("markDuplicates")) markDuplicates = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputDuplicates")) outputDuplicates = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("smoothThermistor")) smoothThermistor = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("radiometricInterpolation")) radiometricInterpolation = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("displayThermistor")) displayThermistor = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("serialComms")) serialComms = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("readThermistor")) readThermistor = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("forceInputGray")) forceInputGray = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("fixDudPixels")) fixDudPixels = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("disableSkimming")) disableSkimming = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			
			if (!v2.second.get_child("<xmlattr>.name").data().compare("captureMode")) captureMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("readMode")) readMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("loadMode")) loadMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("subscribeMode")) subscribeMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("resampleMode")) resampleMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("pollMode")) pollMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");

			if (!v2.second.get_child("<xmlattr>.name").data().compare("loopMode")) loopMode = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("resizeImages")) resizeImages = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("dumpTimestamps")) dumpTimestamps = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("removeDuplicates")) removeDuplicates = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("temporalSmoothing")) temporalSmoothing = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("extremes")) extremes = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("stepChangeTempScale")) stepChangeTempScale = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			
			if (!v2.second.get_child("<xmlattr>.name").data().compare("rectifyImages")) rectifyImages = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("writeImages")) writeImages = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("keepOriginalNames")) keepOriginalNames = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("writeVideo")) writeVideo = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("republishNewTimeStamp")) republishNewTimeStamp = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("drawReticle")) drawReticle = !v2.second.get_child("<xmlattr>.value").data().compare("true");
			if (!v2.second.get_child("<xmlattr>.name").data().compare("autoAlpha")) autoAlpha = !v2.second.get_child("<xmlattr>.value").data().compare("true");

			if (!v2.second.get_child("<xmlattr>.name").data().compare("filterMode")) filterMode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("radiometricBias")) radiometricBias = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("calibrationMode")) calibrationMode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("alternatePeriod")) alternatePeriod = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("inputWidth")) inputWidth = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("inputHeight")) inputHeight = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("serialCommsConfigurationCode")) serialCommsConfigurationCode = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("maxNucInterval")) maxNucInterval = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("serialWriteAttempts")) serialWriteAttempts = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("republishSource")) republishSource = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("outputFormat")) outputFormat = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("device_num")) device_num = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());

			if (!v2.second.get_child("<xmlattr>.name").data().compare("soft_diff_limit")) soft_diff_limit = atoi(v2.second.get_child("<xmlattr>.value").data().c_str());

			if (!v2.second.get_child("<xmlattr>.name").data().compare("filterParam")) filterParam = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("thermistorWindow")) thermistorWindow = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("syncDiff")) syncDiff = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("writeQuality")) writeQuality = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("maxThermistorDiff")) maxThermistorDiff = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("maxIntensityChange")) maxIntensityChange = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
			if (!v2.second.get_child("<xmlattr>.name").data().compare("alpha")) alpha = atof(v2.second.get_child("<xmlattr>.value").data().c_str());
        }

#ifdef _WIN32
		// Substitute tildes if in Windows
		if (folder.size() > 0) {
			if (folder[0] == '~') {
				folder.erase(folder.begin());
				folder = std::getenv("USERPROFILE") + folder;
			}
		}

		if (outputFolder.size() > 0) {
			if (outputFolder[0] == '~') {
				outputFolder.erase(outputFolder.begin());
				outputFolder = std::getenv("USERPROFILE") + outputFolder;
			}
		}
#endif

	}

	return dataValid;

}

#ifndef _BUILD_FOR_ROS_
bool streamerConfig::assignStartingData(streamerData& startupData) {

	debugMode = startupData.debugMode;
	verboseMode = startupData.verboseMode;

	output16bit = startupData.output16bit;
	output8bit = startupData.output8bit;
	outputColor = startupData.outputColor;

	undistortImages = startupData.undistortImages;
	autoTemperature = startupData.autoTemperature;
	
	inputDatatype = startupData.inputDatatype;
	detectorMode = startupData.detectorMode;
	usbMode = startupData.usbMode;

	maxReadAttempts = startupData.maxReadAttempts;
	maxNucInterval = startupData.maxNucInterval;

	normMode = startupData.normMode;
	map = startupData.map;
	
	minTemperature = startupData.minTemperature;
	maxTemperature = startupData.maxTemperature;

	tempGrad = startupData.tempGrad;
	tempIntercept = startupData.tempIntercept;

	framerate = startupData.framerate;
	normFactor = startupData.normFactor;
	fusionFactor = startupData.fusionFactor;
	
	serialPollingRate = startupData.serialPollingRate;
	maxNucThreshold = startupData.maxNucThreshold;

	// ...

	if (startupData.outputFolder.size() > 0) {
		if (startupData.outputFolder.at(startupData.outputFolder.size()-1) == '/') startupData.outputFolder = startupData.outputFolder.substr(0, startupData.outputFolder.size()-1);
	} else {
		ROS_WARN("Specified output folder is 0 characters long - ignoring.");
		startupData.outputFolder = "outputFolder";
	}

	if (verboseMode) { ROS_INFO("outputFolder = (%s)", startupData.outputFolder.c_str()); }

	if ((startupData.writeImages) && (startupData.outputFolder.size() > 0)) {
		// Create necessary folders
		char folderCommand[256];
		sprintf(folderCommand, "mkdir -p %s", startupData.outputFolder.c_str());
		
		int res = system(folderCommand);
		
		if (res == 0) ROS_WARN("system() call returned 0...");
	}

	ROS_INFO("calibrationMode = (%d)", startupData.calibrationMode);

	if (startupData.resizeImages) {
		if ((startupData.desiredRows < 1) || (startupData.desiredRows > MAX_ROWS) || (startupData.desiredCols < 1) || (startupData.desiredCols > MAX_COLS)) {
			ROS_ERROR("Resizing values (%d, %d) invalid.", startupData.desiredCols, startupData.desiredRows);
			startupData.dataValid = false;
			
		} else {
			ROS_INFO("Resizing to (%d x %d)", startupData.desiredCols, startupData.desiredRows);
		}
	}

	if (startupData.undistortImages) { ROS_INFO("Undistorting images..."); }

	if (startupData.normalizationMode == "standard") {
		normMode = 0;
	} else if (startupData.normalizationMode == "low_contrast") {
		normMode = 1;
	}

	if (startupData.writeVideo) {
		if (verboseMode) { ROS_INFO("Has chosen to encode."); }
		
		if (startupData.outputVideo == "outputVideo") {
			ROS_ERROR("outputVideo incorrectly specified...");
			startupData.dataValid = false;
		} else ROS_INFO("outputVideo = (%s)", startupData.outputVideo.c_str());
		
		if (verboseMode) { ROS_INFO("Image format = (%s); image type = (%s)", "avi", startupData.videoType.c_str()); }
	}

	if (inputDatatype == DATATYPE_8BIT) {
		if (verboseMode) { ROS_INFO("Streaming mode: 8-bit"); }
	} else if (inputDatatype == DATATYPE_RAW) {
		if (verboseMode) { ROS_INFO("Streaming mode: 16-bit"); }
	} else if (inputDatatype == DATATYPE_MM) {
		if (verboseMode) { ROS_INFO("Streaming mode: multimodal"); }
	}

	if (startupData.source == "dev") {
		
		if ((framerate < 0) || (framerate > MAX_READ_RATE)) {
			if (verboseMode) { ROS_INFO("Framerate set to (%f) so therefore defaulting to capture mode.", framerate); }
			framerate = MAX_READ_RATE;
			startupData.captureMode = true;
		} else {
			if (verboseMode) { ROS_INFO("Requested framerate (%f) - polling mode", framerate); }
			startupData.pollMode = true;
		}
		
		ROS_INFO("Reading from device (%s)", startupData.capture_device.c_str());
		
	} else if (startupData.source == "file") {
		startupData.readMode = true;
		
		if ((framerate < 0.0) || (framerate > MAX_READ_RATE)) {
			ROS_INFO("Invalid framerate (%f) so defaulting to (%f).", framerate, DEFAULT_READ_RATE);
			framerate = DEFAULT_READ_RATE;
		} else {
			ROS_INFO("Requested framerate = %f", framerate);
		}
		
		ROS_INFO("Reading from a file (%s)", startupData.filename.c_str());
	} else if ((startupData.source == "folder") || (startupData.source == "directory")) {
		
		startupData.loadMode = true;
		
		if ((framerate < 0.0) || (framerate > MAX_READ_RATE)) {
			ROS_WARN("Invalid framerate (%f) so defaulting to (%f).", framerate, DEFAULT_READ_RATE);
			framerate = DEFAULT_READ_RATE;
		} else {
			ROS_INFO("Requested framerate = %f", framerate);
		}
		
		ROS_INFO("Loading images from a folder (%s)", startupData.folder.c_str());
		
		
	} else if (startupData.source == "topic") {
		
		ROS_INFO("Subscribing to topic (%s) ", startupData.topicname.c_str());
		
		if ((framerate < 0.0) || (framerate > MAX_READ_RATE)) {
			ROS_WARN("Invalid framerate (%f) so defaulting to subscribe mode.", framerate);
			framerate = DEFAULT_READ_RATE;
			startupData.subscribeMode = true;
		} else {
			ROS_INFO("Requested framerate = %f (resample mode)", framerate);
			startupData.resampleMode = true;
		}
		
	}

	if (startupData.loopMode == true) ROS_INFO("Option to loop has been selected.");

	startupData.device_num = atoi(&(startupData.capture_device.c_str(), startupData.capture_device.at(startupData.capture_device.length()-1)));
	getMapping(map, startupData.extremes, startupData.mapCode, startupData.mapParam);

	if (startupData.outputType == OUTPUT_TYPE_CV_8UC3) {
		if (startupData.outputFormatString == "pgm") ROS_WARN("PGM cannot write as CV_8UC3...");
	} else if (startupData.outputType == OUTPUT_TYPE_CV_8UC1) { 
		// ...
	} else if (startupData.outputType == OUTPUT_TYPE_CV_16UC1) {
		if ((startupData.outputFormatString == "jpg") || (startupData.outputFormatString == "bmp")) ROS_WARN("JPG/BMP cannot write as CV_16UC1...");
	} else ROS_WARN("Unrecognized output format (%d)", startupData.outputType);

	switch (startupData.outputFormat) {
	case 0:
		startupData.outputFormatString = "jpg";
		break;
	case 1:
		startupData.outputFormatString = "pgm";
		break;
	case 2:
		startupData.outputFormatString = "bmp";
		break;
	case 3:
		startupData.outputFormatString = "ppm";
		break;
	case 4:
		startupData.outputFormatString = "png";
		break;
	default:
		startupData.outputFormatString = "png";
		break;
	}
	
	if (startupData.intrinsics != "intrinsics") {
		startupData.intrinsicsProvided = true;
		if (verboseMode) { ROS_INFO("Intrinsics at (%s) selected.", startupData.intrinsics.c_str()); }
		if ((startupData.inputWidth != 0) && (startupData.inputHeight != 0)) ROS_WARN("Provided image dimensions will be ignored because of provided intrinsics file.");
	} else {
		if ((startupData.inputWidth != 0) && (startupData.inputHeight != 0)) {
			ROS_INFO("Provided image dimensions (%d, %d) will be used.", startupData.inputWidth, startupData.inputHeight);
			startupData.imageDimensionsSpecified = true;
		} ROS_WARN("No intrinsics or image dimensions provided. Will attempt to estimate camera size...");
	}

	if (startupData.extrinsics != "extrinsics") {
		ROS_INFO("Extrinsics at %s selected.", startupData.extrinsics.c_str());
		startupData.addExtrinsics = true;
		if (startupData.camera_number < 0) {
			ROS_WARN("Invalid camera number selected (%d) so defaulting to (0).", startupData.camera_number);
			startupData.camera_number = 0;
		} ROS_INFO("Camera number (%d).", startupData.camera_number);
	}

	startupData.soft_diff_limit = (unsigned long) (startupData.syncDiff * 1000000000.0);

	startupData.outputFileParams.clear();
	int val;
	if (startupData.outputFormatString == "png") {
		startupData.outputFileParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
		val = int((1.0-startupData.writeQuality) * 9.0);
		startupData.outputFileParams.push_back(val);
	} else if (startupData.outputFormatString == "jpg") {
		startupData.outputFileParams.push_back(CV_IMWRITE_JPEG_QUALITY);
		val = int(startupData.writeQuality * 100.0);
		startupData.outputFileParams.push_back(val);
	}

	if (verboseMode) { ROS_INFO("Checkpoint (%d) reached.", 0); }

	//check for valid republishSource
    switch (startupData.republishSource) {
    case REPUBLISH_CODE_8BIT_MONO:
        ROS_INFO("Republishing mono image as %s", startupData.republishTopic.c_str() );
        break;
    case REPUBLISH_CODE_8BIT_COL:
        ROS_INFO("Republishing color image as %s", startupData.republishTopic.c_str() );
        break;
    case REPUBLISH_CODE_16BIT:
        ROS_INFO("Republishing 16bit image as %s", startupData.republishTopic.c_str() );
        break;
    default:
        startupData.republishSource = NO_REPUBLISH_CODE;
        break;
    }

    if (startupData.republishSource != NO_REPUBLISH_CODE){
        ROS_INFO("Republish Code: %d", startupData.republishSource);
    }

	if (verboseMode) { ROS_INFO("Checkpoint (%d) reached.", 1); }

	int modeCount = 0;
	
	if (startupData.captureMode) modeCount++;
	if (startupData.pollMode) modeCount++;
	if (startupData.readMode) modeCount++;
	if (startupData.loadMode) modeCount++;
	if (startupData.subscribeMode) modeCount++;
	if (startupData.resampleMode) modeCount++;
	
	if (modeCount == 0) {
		ROS_ERROR("Either a device, file or topic should be specified for streaming.");
		startupData.dataValid = false;
	} else if (modeCount > 1) {
		ROS_ERROR("Either a device, file or topic should be specified - not more than one.");
		startupData.dataValid = false;
	}
	
	if ((framerate < -1.0) || (framerate > MAX_READ_RATE)) framerate = DEFAULT_READ_RATE;

	if (verboseMode) { ROS_INFO("Checkpoint (%d) reached.", 2); }

	return startupData.dataValid;

}
#endif

inputStream::inputStream() {
	cMapping = new cScheme(DEFAULT_COLORSCHEME_CODE);
	colormap_index = DEFAULT_COLORSCHEME_CODE;

	minTemp = DEFAULT_MIN_TEMP; 
	maxTemp = DEFAULT_MAX_TEMP;

	autoscaleTemps = true;

	rawImage = new cv::Mat();
	scaledImage = new cv::Mat();
	_8bitImage = new cv::Mat();
}

bool inputStream::accessLatestRawFrame(cv::Mat& latestFrame) {
	if (rawImage->rows == 0) return false;
	latestFrame = *rawImage;
	return true;
}

bool inputStream::accessLatest8bitFrame(cv::Mat& latestFrame) {
	if (_8bitImage->rows == 0) return false;
	latestFrame = *_8bitImage;
	return true;
}

void inputStream::displayFrame() {
	if (displayImage->rows != 0) {
		!pauseMode ? cv::imshow("display", *displayImage) : 0;
	} else if (_8bitImage->rows != 0) {
		!pauseMode ? cv::imshow("display", *_8bitImage) : 0;
	} else {
		!pauseMode ? cv::imshow("display", *rawImage) : 0;
	}
	
	char key = cv::waitKey(1);
	if (key == 'q') isValid = false;
}

bool inputStream::processFrame() {

	if (scaledImage->rows == 0) {
		scaledImage = new cv::Mat(rawImage->rows, rawImage->cols, CV_16UC1);
		_8bitImage = new cv::Mat(rawImage->rows, rawImage->cols, CV_8UC1);
		displayImage = new cv::Mat(rawImage->rows, rawImage->cols, CV_8UC3);
	}

	if (autoscaleTemps) {
		normalize_16(*scaledImage, *rawImage);
	} else {
		double minLevel = minTemp * 100.0;
		double maxLevel = maxTemp * 100.0;
		normalize_16(*scaledImage, *rawImage, minLevel, maxLevel);
	}
	
	down_level(*_8bitImage, *scaledImage);

	return 0;
}

bool inputStream::writeImageToDisk() {
	if (wantsToOutput) {
		char imFilename[256];
		sprintf(imFilename, "%s/frame%06d.png", output_directory, FrameCounter1);
		std::string imageFilename(imFilename);

		if (writeInColor && (displayImage->rows != 0)) {
			cv::imwrite(imageFilename, *displayImage);
		} else if (writeInColor && (_8bitImage->rows != 0)) {
			cv::imwrite(imageFilename, *_8bitImage);
		} else {
			cv::imwrite(imageFilename, *rawImage);
		}

		return true;
	}
	return false;
}

void streamerNode::assignDefaultCameraInfo() {
	
	if (configData.verboseMode) { ROS_INFO("Entered.."); }

	globalCameraInfo.imageSize = cv::Mat(1, 2, CV_16UC1);
	globalCameraInfo.imageSize.at<unsigned short>(0, 1) = DEFAULT_IMAGE_HEIGHT;
	globalCameraInfo.imageSize.at<unsigned short>(0, 0) = DEFAULT_IMAGE_WIDTH;
	
	globalCameraInfo.cameraMatrix = cv::Mat::eye(3, 3, CV_64FC1);
	globalCameraInfo.distCoeffs = cv::Mat::zeros(1, 8, CV_64FC1);
	globalCameraInfo.newCamMat = cv::Mat::eye(3, 3, CV_64FC1);
	globalCameraInfo.cameraSize = cv::Size(globalCameraInfo.imageSize.at<unsigned short>(0, 0), globalCameraInfo.imageSize.at<unsigned short>(0, 1));

	if (configData.verboseMode) { ROS_INFO("globalCameraInfo.cameraSize = (%d, %d)", globalCameraInfo.cameraSize.width, globalCameraInfo.cameraSize.height); }
	
}

#ifdef _BUILD_FOR_ROS_
streamerNode::streamerNode(ros::NodeHandle& nh, streamerData startupData) :
#else
streamerNode::streamerNode(streamerData startupData) :
#endif
	canRadiometricallyCorrect(false),
	deviceCreated(false),
	recordedThermistorReadings(0), 
	settingsDisabled(false), 
	currentNucProtectionMode(false), 
	wantsToDisableNuc(false), 
	lastIsDuplicate(true), 
	firstCall(true), 
	updateNucInterval(true), 
	updateDetectorMode(true), 
	updateUSBMode(true), 
	altStatus(true), 
	performingNuc(false), 
	alphaChanged(true), 
	readyToPublish(false), 
	isActuallyGray(false), 
	videoInitialized(false), 
	videoValid(true), 
	firstFrame(true), 
    centerPrincipalPoint(true), 
	firstServerCallbackProcessed(false), 
	pastMeanIndex(-1), 
	medianPercentile(0.50), 
	alternateCounter(0), 
	fusionFactor(0.8), 
	fileCount(0), 
	writeIndex(0), 
	frameCounter(0), 
	lastWritten(-1), 
	lastThermistorReading(-99.0), 
	lastMedian(-1.0), 
	lastLowerLimit(-1.0), 
	lastUpperLimit(-1.0), 
	oldMaxDiff(-1.0), 
	minVal(65535.0), 
	maxVal(0.0)
{
	
	configData = startupData;

	#ifndef _BUILD_FOR_ROS_
	bridgeReplacement = new cv::Mat();
	#endif
	
#ifdef _BUILD_FOR_ROS_
	dodgeTime.sec = 0;
	dodgeTime.nsec = 0;
#endif

	if (configData.addExtrinsics) {
		getRectification();

#ifdef _BUILD_FOR_ROS_
		updateCameraInfoExtrinsics();
#endif
	}
	
	#ifdef _BUILD_FOR_ROS_
	sprintf(nodeName, "%s", ros::this_node::getName().substr(1).c_str());
	#else
	sprintf(nodeName, "/%s", "THERMALVIS_STREAMER");
	#endif

	if (configData.verboseMode) { ROS_INFO("Initializing node (%s)", nodeName); }
	
	if (configData.radiometryFile != "radiometryFile") {
		// Does file exist?
		
		cv::Mat sensorLimits, graylevelLimits, mappingMatrix;
		
		if (configData.verboseMode) { ROS_INFO("About to read from (%s)", configData.radiometryFile.c_str()); }
		
		cv::FileStorage fs(configData.radiometryFile, cv::FileStorage::READ);
		fs["sensorLimits"] >> sensorLimits;
		fs["graylevelLimits"] >> graylevelLimits;
		fs["mappingMatrix"] >> mappingMatrix;
		fs.release();
		
		if ((sensorLimits.rows == 0) || (graylevelLimits.rows == 0) || (mappingMatrix.rows == 0)) {
			ROS_ERROR("Provided radiometry file is invalid. Please check.");
			setValidity(false);
			prepareForTermination();
			return;
		}
		
		radMapper.update(mappingMatrix, sensorLimits.at<double>(0,0), sensorLimits.at<double>(0,1), graylevelLimits.at<double>(0,0), graylevelLimits.at<double>(0,1));
		
		canRadiometricallyCorrect = true;
	
	}
	
#ifndef _WIN32
	if (configData.serialComms) {
		if (configData.verboseMode) { ROS_INFO("Configuring serial comms..."); }
		if (!configureSerialComms()) {
			ROS_ERROR("Serial comms configuration failed. Shutting down.");
			setValidity(false);
			prepareForTermination();
			return;
		}
	}
#endif

	if (configData.verboseMode) { ROS_INFO("configData.serialPollingRate = (%f)", configData.serialPollingRate); }
	
#ifdef _BUILD_FOR_ROS_
	if (configData.serialPollingRate > 0.1) {
		if (configData.verboseMode) { ROS_INFO("Initializing serial timer at (%f)", 1.0 / ((double) configData.serialPollingRate)); }
		serial_timer = nh.createTimer(ros::Duration(1.0 / ((double) configData.serialPollingRate)), &streamerNode::serialCallback, this);
	} else {
		if (configData.verboseMode) { ROS_INFO("Initializing serial timer at (%f)", DEFAULT_SERIAL_POLLING_RATE); }
		serial_timer = nh.createTimer(ros::Duration(DEFAULT_SERIAL_POLLING_RATE), &streamerNode::serialCallback, this);
	}
	
	it = new image_transport::ImageTransport(nh);
#endif

	string info_name = configData.topicname.substr(0, configData.topicname.find_last_of("/") + 1) + "camera_info";
	if (configData.verboseMode) { ROS_INFO("configData.topicname = (%s)", info_name.c_str()); }
	
	if (configData.calibrationMode) {
#ifdef _BUILD_FOR_ROS_
		ros::Duration(1.0).sleep(); // wait a little bit to ensure that the serial comms has been configured..
#endif
	}
	
	if (configData.verboseMode) { ROS_INFO("Initializing camera (%s)", configData.topicname.c_str()); }
	
#ifndef _WIN32
	mainVideoSource = new streamerSource;
#endif

	std::string camera_name;
	
	if (configData.outputFolder.size() == 0) {
		configData.outputFolder = configData.read_addr + configData.outputFolder;
	} else if (configData.outputFolder[0] != '/') {
		configData.outputFolder = configData.read_addr + configData.outputFolder;
	}
	
	configData.outputTimeFile = configData.outputFolder + "-timestamps.txt";
	
	if (configData.dumpTimestamps) {
		ofs.open(configData.outputTimeFile.c_str());
	}
	
	if (configData.intrinsicsProvided) {
		
		if (configData.verboseMode) { ROS_INFO("Reading in calibration data"); }
		
		if (configData.intrinsics[0] != '/') {
			configData.intrinsics = configData.read_addr + configData.intrinsics;
		}
		
		if (configData.verboseMode) { ROS_INFO("intrinsics = %s", configData.intrinsics.c_str()); }
		
		// http://www.mathpirate.net/log/2009/11/26/when-in-doubt-link-debug/
		cv::Mat dummyImage(300, 300, CV_8UC1);
		GaussianBlur(dummyImage, dummyImage, cv::Size(5, 5), 1.5);
		
		
		cv::FileStorage fs(configData.intrinsics, cv::FileStorage::READ);
		fs["imageSize"] >> globalCameraInfo.imageSize;
		fs["cameraMatrix"] >> globalCameraInfo.cameraMatrix;
		fs["distCoeffs"] >> globalCameraInfo.distCoeffs;
		fs["newCamMat"] >> globalCameraInfo.newCamMat;
		fs.release();
		
		if (configData.verboseMode) { ROS_INFO("Calibration data read."); }

			//HGH
			if (globalCameraInfo.cameraMatrix.empty()){
				ROS_ERROR("Intrinsics file %s invalid! Please check path and filecontent...\n", configData.intrinsics.c_str());
			}
		
		if (configData.verboseMode) { ROS_INFO("Establishing size (%d, %d).", globalCameraInfo.imageSize.at<unsigned short>(0, 0), globalCameraInfo.imageSize.at<unsigned short>(0, 1)); }
		
		globalCameraInfo.cameraSize = cv::Size(globalCameraInfo.imageSize.at<unsigned short>(0, 0), globalCameraInfo.imageSize.at<unsigned short>(0, 1));
		if (configData.verboseMode) { ROS_INFO("globalCameraInfo.cameraSize = (%d, %d)", globalCameraInfo.cameraSize.width, globalCameraInfo.cameraSize.height); }
		
		//globalCameraInfo.newCamMat = getOptimalNewCameraMatrix(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalCameraInfo.cameraSize, configData.alpha, globalCameraInfo.cameraSize, validPixROI, centerPrincipalPoint);
		
		
		if (configData.verboseMode) { ROS_INFO("Global params determined."); }
		
		
	} else if (configData.imageDimensionsSpecified) {
		
		if (configData.verboseMode) { ROS_INFO("Assigning default intrinsics but with specified image size"); }
		
		assignDefaultCameraInfo();

		globalCameraInfo.imageSize.at<unsigned short>(0, 1) = configData.inputHeight;
		globalCameraInfo.imageSize.at<unsigned short>(0, 0) = configData.inputWidth;
		globalCameraInfo.cameraSize = cv::Size(globalCameraInfo.imageSize.at<unsigned short>(0, 0), globalCameraInfo.imageSize.at<unsigned short>(0, 1));
		if (configData.verboseMode) { ROS_INFO("A: globalCameraInfo.cameraSize = (%d, %d)", globalCameraInfo.cameraSize.width, globalCameraInfo.cameraSize.height); }
		
		if (configData.verboseMode) { ROS_INFO("Assigned."); }
		
	} else {
		if (configData.verboseMode) { ROS_INFO("Assigning default intrinsics..."); }

		assignDefaultCameraInfo();

		if (configData.verboseMode) { ROS_INFO("Default intrinsics assigned."); }
	}
	
	
	//HGH
	if (configData.autoAlpha) {
		
		if (configData.intrinsicsProvided) {
			if (configData.verboseMode) { ROS_INFO("Finding auto alpha..."); }
			configData.alpha = findBestAlpha(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalCameraInfo.cameraSize);
			//HGH
			if (configData.verboseMode) { ROS_INFO("Optimal alpha: (%f)", configData.alpha); }
		} else {
			ROS_WARN("Cannot estimate an appropriate alpha coefficient because no intrinsics were provided.");
		}
		
	}

	if (configData.verboseMode) { ROS_INFO("Debug (%d)", 321); }

	if (configData.addExtrinsics) {
		if (configData.verboseMode) { ROS_INFO("Reading in extrinsics..."); }

			char rotation_name[256], translation_name[256];

			sprintf(rotation_name, "R_%d", configData.camera_number);
			sprintf(translation_name, "T%d", configData.camera_number);

			// http://www.mathpirate.net/log/2009/11/26/when-in-doubt-link-debug/
			cv::Mat dummyImage(300, 300, CV_8UC1);
			GaussianBlur(dummyImage, dummyImage, cv::Size(5, 5), 1.5);

			cv::FileStorage fs(configData.extrinsics, cv::FileStorage::READ);

			//HGH
			fs[rotation_name] >> globalCameraInfo.R;
			fs[translation_name] >> globalCameraInfo.T;

			//used to calculate the individual rotation and projection matrices for rectification depending on alpha
			fs["R"] >> globalExtrinsicsData.R;
			fs["T"] >> globalExtrinsicsData.T;

			//we need also both camera matrices and distortion coefficients
			fs["cameraMatrix0"] >> globalExtrinsicsData.cameraMatrix0;
			fs["cameraMatrix1"] >> globalExtrinsicsData.cameraMatrix1;
			fs["distCoeffs0"] >> globalExtrinsicsData.distCoeffs0;
			fs["distCoeffs1"] >> globalExtrinsicsData.distCoeffs1;

	fs.release();

			if (globalExtrinsicsData.R.empty()){
				ROS_ERROR("Extrinsics file %s invalid! Please check path and filecontent...\n", configData.extrinsics.c_str());
			}

			globalCameraInfo.cameraSize = cv::Size(globalCameraInfo.imageSize.at<unsigned short>(0, 0), globalCameraInfo.imageSize.at<unsigned short>(0, 1));
			if (configData.verboseMode) { ROS_INFO("X: globalCameraInfo.cameraSize = (%d, %d)", globalCameraInfo.cameraSize.width, globalCameraInfo.cameraSize.height); }

			getRectification();

	} else {

		if (configData.verboseMode) { ROS_INFO("Debug (%d)", 322); }

		globalCameraInfo.R = cv::Mat::eye(3, 3, CV_64FC1);
		globalCameraInfo.T = cv::Mat::zeros(3, 1, CV_64FC1);
	}

#ifdef _BUILD_FOR_ROS_
	if (configData.verboseMode) { ROS_INFO("calling assignCameraInfo() from streamerNode()..."); }
	assignCameraInfo();
#endif

	if (configData.verboseMode) { ROS_INFO("Initializing video source..."); }

#ifndef _WIN32
	mainVideoSource->initialize_video_source();
#endif

	colourMap.load_standard(configData.mapCode, configData.mapParam);
	
#ifdef _BUILD_FOR_ROS_
	refreshCameraAdvertisements();
#endif

	char cameraInfoName[256];
	sprintf(cameraInfoName, "thermalvis/%s/set_camera_info", nodeName);

#ifdef _BUILD_FOR_ROS_
	ros::ServiceServer set_camera_info = nh.advertiseService(cameraInfoName, &streamerNode::setCameraInfo, this);
#endif

	if (configData.framerate > 0.0) {
		configData.pauseMode = false;
#ifdef _BUILD_FOR_ROS_
		timer = nh.createTimer(ros::Duration(1.0 / ((double) configData.framerate)), &streamerNode::timerCallback, this);
#endif
	} else {
		configData.pauseMode = true;
#ifdef _BUILD_FOR_ROS_
		timer = nh.createTimer(ros::Duration(1.0 / 1.0), &streamerNode::timerCallback, this);
#endif
	}
	
	// Configure log files
	callLogFile = configData.read_addr + "/nodes/streamer/log/call_log.txt";
	retrieveLogFile = configData.read_addr + "/nodes/streamer/log/retrieve_log.txt";
	internalLogFile = configData.read_addr + "/nodes/streamer/log/internal_log.txt";
	writeLogFile = configData.read_addr + "/nodes/streamer/log/write_log.txt";
	duplicatesLogFile = configData.outputFolder + "-duplicates.txt";
	thermistorLogFile = configData.outputFolder + "-thermistor.txt";
	
#ifdef _BUILD_FOR_ROS_
	if (configData.verboseMode) { ROS_INFO("Establishing server callback..."); }
	f = boost::bind (&streamerNode::serverCallback, this, _1, _2);
    server.setCallback (f);
#endif

#ifdef _BUILD_FOR_ROS_
    if (configData.subscribeMode || configData.resampleMode) {
		if (configData.verboseMode) { ROS_INFO("syncMode: (%d)", configData.syncMode); }
		if (configData.syncMode == SYNCMODE_HARD) {
			camera_sub = it->subscribeCamera(configData.topicname, 1, &streamerNode::handle_camera, this);
		} else if (configData.syncMode == SYNCMODE_SOFT) {
			info_sub = nh.subscribe<sensor_msgs::CameraInfo>(info_name, 1, &streamerNode::handle_info, this);
			image_sub = it->subscribe(configData.topicname, 1, &streamerNode::handle_image, this);
		} else if (configData.syncMode == SYNCMODE_IMAGEONLY) {
			image_sub = it->subscribe(configData.topicname, 1, &streamerNode::handle_image, this);
		}
	}
#endif
	
	if (configData.externalNucManagement != "") {
		
#ifdef _BUILD_FOR_ROS_
		lastFlagReceived = ros::Time::now();
#else
		lastFlagReceived = boost::posix_time::microsec_clock::local_time();
#endif

#ifdef _BUILD_FOR_ROS_
		ROS_INFO("Subscribing to external nuc management flag (%s)", configData.externalNucManagement.c_str());
		nuc_management_sub = nh.subscribe<std_msgs::Float32>(configData.externalNucManagement, 1, &streamerNode::handle_nuc_instruction, this);
#endif
	}
    
#ifdef _BUILD_FOR_ROS_
    // Wait for the first server callback to be processed before continuing..
    while (!firstServerCallbackProcessed) { };
#endif

    if (configData.dumpTimestamps) {
		ofs_call_log.open(callLogFile.c_str());
		ofs_retrieve_log.open(retrieveLogFile.c_str());
		ofs_internal_log.open(internalLogFile.c_str());
		ofs_write_log.open(writeLogFile.c_str());
	}
	
	if (configData.outputDuplicates) {
		if (configData.verboseMode) { ROS_INFO("Outputting duplicates to (%s)", duplicatesLogFile.c_str()); }
		ofs_duplicates_log.open(duplicatesLogFile.c_str());
	}
}

void streamerNode::prepareForTermination() {
	if (ofs.is_open()) ofs.close();
	if (ofs_call_log.is_open()) ofs_call_log.close();
	if (ofs_retrieve_log.is_open()) ofs_retrieve_log.close();
	if (ofs_internal_log.is_open()) ofs_internal_log.close();
	if (ofs_write_log.is_open()) ofs_write_log.close();
	if (ofs_duplicates_log.is_open()) ofs_duplicates_log.close();
	if (ofs_thermistor_log.is_open()) ofs_thermistor_log.close();
	
#ifndef _WIN32
	if (configData.serialComms) close(mainfd);
#endif
	
	releaseDevice();	
}

#ifndef _WIN32
bool streamerNode::configureSerialComms() {
	
	
	struct termios options;
   
	mainfd = open_port();
	
	//sleep(2); //required to make flush work, for some reason
	//tcflush(mainfd,TCIOFLUSH);
	
	int fcntl_ret = fcntl(mainfd, F_SETFL, FNDELAY); /* Configure port reading */
	//int fcntl_ret = fcntl(mainfd, F_SETFL, 0);
	
	if (fcntl_ret != 0) { ROS_ERROR("fcntl_ret = (%d)", fcntl_ret); }
	
	//sleep(2); //required to make flush work, for some reason
	//tcflush(mainfd,TCIOFLUSH);
	
	if (configData.verboseMode) { displayTermiosData(options); }
	
	//int tcgetattr_ret = tcgetattr(mainfd, &options); 					/* Get the current options for the port */
	//ROS_ERROR("tcgetattr_ret = (%d)", tcgetattr_ret);
	
	if (0) { // Attempt to write with 1-1 converter
		
        bzero(&options, sizeof(options));
        options.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD; // cflag is common flag?
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        
        /* set input mode (non-canonical, no echo,...) */
        options.c_lflag = 0;
         
        options.c_cc[VTIME]    = 0;   /* inter-character timer unused */
        options.c_cc[VMIN]     = 11;   /* blocking read until 5 chars received */
        
        tcflush(mainfd, TCIFLUSH);
        tcsetattr(mainfd,TCSANOW,&options);
        tcflush(mainfd, TCIFLUSH);
	} else {
		// Works for the Serial Hub, and writes properly with 1-1 converter
		
		//bzero(&options, sizeof(options));
		
		//options.c_cflag &= B115200;
		
		int cfsetispeed_ret = cfsetispeed(&options, B115200); /* Set the baud rates to 115200 */ // can change "i" to 2400 and it still works, not so with "o" ...
		if (cfsetispeed_ret != 0) { ROS_ERROR("cfsetispeed_ret = (%d)", cfsetispeed_ret); }
		int cfsetospeed_ret = cfsetospeed(&options, B115200);
		if (cfsetospeed_ret != 0) { ROS_ERROR("cfsetospeed_ret = (%d)", cfsetospeed_ret); }
		
		
		
		// should have 1 stop bit, 115200 baud, 8 bits, no parity, no flow control
		
									   /* Enable the receiver and set local mode */
	    
		
	
		
		// Hack: additional settings to align with GTKTERM:
		
		if (configData.serialCommsConfigurationCode == SERIAL_COMMS_CONFIG_DEFAULT) {
			
			//ROS_ERROR("Assigning here...");
			
			if (1) {
				
				bzero(&options, sizeof(options));
				
				options.c_iflag = 0;
				options.c_oflag = 0;
				options.c_cflag = 0;
				options.c_lflag = 0;
				
				// http://man7.org/linux/man-pages/man3/termios.3.html
				
				//options.c_cflag &= B115200;
				
				cfsetispeed(&options, B115200);
				cfsetospeed(&options, B115200);
		
				//int cfsetispeed_ret = cfsetispeed(&options, B115200); /* Set the baud rates to 115200 */ // can change "i" to 2400 and it still works, not so with "o" ...
				//ROS_ERROR("cfsetispeed_ret = (%d)", cfsetispeed_ret);
				//int cfsetospeed_ret = cfsetospeed(&options, B115200);
				//ROS_ERROR("cfsetospeed_ret = (%d)", cfsetospeed_ret);
				
				
				
				// c_iflag : http://www.delorie.com/gnu/docs/glibc/libc_362.html
				options.c_iflag &= ~INPCK;
				options.c_iflag |= IGNPAR;
				options.c_iflag &= ~PARMRK;
				options.c_iflag &= ~ISTRIP;
				options.c_iflag |= IGNBRK;
				options.c_iflag &= ~BRKINT;
				options.c_iflag &= ~IGNCR;
				options.c_iflag &= ~ICRNL;
				options.c_iflag &= ~INLCR;
				options.c_iflag &= ~IXOFF;
				options.c_iflag &= ~IXON;
				options.c_iflag &= ~IXANY;
				options.c_iflag &= ~IMAXBEL;
				// UNUSED:
				options.c_iflag &= ~IUCLC;
				options.c_iflag &= ~IUTF8;
				
				
				// c_oflag : http://www.delorie.com/gnu/docs/glibc/libc_363.html
				options.c_oflag &= ~OPOST;
				options.c_oflag &= ~ONLCR;
				// UNUSED: oxtabs, onoeot
				options.c_oflag &= ~OLCUC;
				options.c_oflag &= ~OCRNL;
				options.c_oflag &= ~ONOCR;
				options.c_oflag &= ~ONLRET;
				options.c_oflag &= ~OFILL;
				options.c_oflag &= ~OFDEL;
				
				options.c_oflag |= NL0;
				options.c_oflag |= CR0;
				options.c_oflag |= TAB0;
				options.c_oflag |= BS0;
				options.c_oflag |= VT0;
				options.c_oflag |= FF0;
				
				
				// c_cflag : http://www.delorie.com/gnu/docs/glibc/libc_364.html
				options.c_cflag |= CLOCAL;
				options.c_cflag &= ~HUPCL;
				options.c_cflag |= CREAD;
				options.c_cflag &= ~CSTOPB; 
				options.c_cflag &= ~PARENB;
				options.c_cflag &= ~PARODD;
				options.c_cflag &= ~CSIZE;
				options.c_cflag |= CS8;
				// UNUSED: cs5, cs6, cs7, ccts_oflow, crts_iflow, mdmbuf, cignore
				//options.c_cflag &= ~CRTSCTS;
				options.c_cflag &= ~CRTSCTS;
				
				
				// c_lflag : http://www.delorie.com/gnu/docs/glibc/libc_365.html
				options.c_lflag &= ~ICANON;
				options.c_lflag &= ~ECHO;
				options.c_lflag &= ~ECHOE;
				options.c_lflag &= ~ECHOPRT;
				options.c_lflag &= ~ECHOK;
				options.c_lflag &= ~ECHOKE;
				options.c_lflag &= ~ECHONL;
				options.c_lflag &= ~ECHOCTL;
				options.c_lflag &= ~ISIG;
				options.c_lflag &= ~IEXTEN;
				options.c_lflag &= ~NOFLSH;
				options.c_lflag &= ~TOSTOP;
				// UNUSED: altwerase, flusho, nokerninfo, pendin
				options.c_lflag &= ~XCASE;		
				
				options.c_ispeed = B115200;
				options.c_ospeed = B115200;
				
				
				options.c_cc[0] = 79;
				options.c_cc[1] = 215;
				options.c_cc[2] = 212;
				options.c_cc[3] = 255;
				options.c_cc[4] = 127;
				
				options.c_cc[5] = 0;
				options.c_cc[6] = 0;
				options.c_cc[7] = 88;
				options.c_cc[8] = 42;
				options.c_cc[9] = 121;
				
				options.c_cc[10] = 106;
				options.c_cc[11] = 96;
				options.c_cc[12] = 127;
				options.c_cc[13] = 0;
				options.c_cc[14] = 0;
				
				options.c_cc[15] = 104;
				options.c_cc[16] = 41;
				options.c_cc[17] = 121;
				options.c_cc[18] = 106;
				options.c_cc[19] = 96;
				
				options.c_cc[20] = 127;
				options.c_cc[21] = 0;
				options.c_cc[22] = 0;
				options.c_cc[23] = 16;
				options.c_cc[24] = 16;
				
				options.c_cc[25] = 121;
				options.c_cc[26] = 106;
				options.c_cc[27] = 96;
				options.c_cc[28] = 127;
				options.c_cc[29] = 0;
				
				options.c_cc[30] = 0;
				options.c_cc[31] = 144;
				
				
				//options.c_cc[VTIME]    = 0;   /* inter-character timer unused */
				//options.c_cc[VMIN]     = 1;   /* blocking read until 1 char received */

				tcflush(mainfd, TCOFLUSH );
				tcflush(mainfd, TCIFLUSH);
				
				int yes = 1;
				
				ioctl(mainfd, TIOCCONS, &yes);
				
				// Taken from : http://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
						
			} else {
				
				//ROS_ERROR("Old ones...");
				
				options.c_cflag |= (CLOCAL | CREAD);
				options.c_cflag &= ~PARENB; /* Mask the character size to 8 bits, no parity */ // changed to PARENB (no effect in streamer)
				options.c_cflag |= CSTOPB;
				options.c_cflag &= ~CSIZE;
				options.c_cflag |=  CS8; /* Select 8 data bits */ // changing to 7 breaks...
				options.c_cflag &= ~CRTSCTS; /* Disable hardware flow control */  
				
				// Try to set some of these...
				options.c_oflag = 0;
			 
											 /* Enable data to be processed as raw input */
				options.c_lflag &= ~(ICANON | ECHO | ISIG);
				//options.c_lflag = 0;
				
				options.c_cflag &= ~CSTOPB;
				options.c_cflag &= ~HUPCL;
				
				
				options.c_cflag &= IGNBRK;
				options.c_cflag &= IGNPAR;	
				//options.c_cflag |= IGNBRK;
				//options.c_cflag |= IGNPAR;	
				
				options.c_cflag &= ~ICRNL;
				options.c_cflag &= ~IXON;
				options.c_cflag &= ~IEXTEN;
				options.c_cflag &= ~ECHOE;
				options.c_cflag &= ~ECHOK;
				options.c_cflag &= ~ECHOCTL;
				options.c_cflag &= ~ECHOKE;
				
				
			}
			

			
					
		} else {
			if (configData.verboseMode) { ROS_INFO("Using modified serial comms configuration settings..."); }
		}
		
		if (configData.verboseMode) { displayTermiosData(options); }
		   
		/* Set the new options for the port */
		//int tcsetattr_ret = tcsetattr(mainfd, TCSAFLUSH, &options);
		int tcsetattr_ret = tcsetattr(mainfd, TCSANOW, &options);
		
		
		if (tcsetattr_ret != 0) { ROS_ERROR("tcsetattr_ret = (%d)", tcsetattr_ret); }
		
		//tcsetattr(mainfd, TCSAFLUSH, &options);
		//sleep(2); //required to make flush work, for some reason
		tcflush(mainfd,TCIOFLUSH);
		
	}
	
	
	
	
	
	{
		if (configData.verboseMode) { ROS_INFO("SerialComms: Opening shutter..."); }
		char localCommand[256];
		sprintf(localCommand, "open");
		if (!sendSerialCommand(localCommand, configData.serialWriteAttempts)) ROS_WARN("Serial command (%s) failed after (%d) attempts", localCommand, configData.serialWriteAttempts);
	}
	
	{
		// Trying to clear serial comms buffer...
		int n;
		char buff[SERIAL_BUFF_SIZE];	
		n = read(mainfd, buff, SERIAL_BUFF_SIZE);
		
		if (configData.verboseMode) { ROS_INFO("Cleared (%d) characters from buffer: (%s)", n, buff); }
		
	}
	
	int statusNum = 0;
	
	
	
	if (configData.disableSkimming) {
		// Check current SKIMACTIVE status
		char localCommand[256];
		sprintf(localCommand, "SKIMACTIVE");
		
		if (1) { ROS_INFO("SerialComms: Disabling mean-shifting..."); }
		if (!sendSerialCommand(localCommand, 1)) ROS_WARN("Serial command (%s) failed after (%d) attempts", localCommand, 1);
		
		ros::Duration(1.0).sleep();
		
		char buff[SERIAL_BUFF_SIZE];	
		read(mainfd, buff, SERIAL_BUFF_SIZE);
		//ROS_WARN("Message received = (%s)\n", buff);
		//printf("%s\n", buff);
		
		//istringstream ss_buff( &buff[103]);
		
		statusNum = atof(&buff[17]);
		if (configData.verboseMode) { ROS_INFO("SKIMACTIVE status = (%d)", statusNum); }
		
	}
	
	ros::Duration(1.0).sleep();
					
	if (configData.disableSkimming) {
		char localCommand[256];
		sprintf(localCommand, "SKIMACTIVE 0");
		
		if (configData.verboseMode) { ROS_INFO("SerialComms: Verifying disabling of mean-shifting..."); }
		if (!sendSerialCommand(localCommand, 1)) ROS_WARN("Serial command (%s) failed after (%d) attempts", localCommand, 1);

	}
	
	{
		char localCommand[256];
		sprintf(localCommand, "SAVE");
		
		if (configData.verboseMode) { ROS_INFO("SerialComms: Saving current settings..."); }
		if (!sendSerialCommand(localCommand, 1)) ROS_WARN("Serial command (%s) failed after (%d) attempts", localCommand, 1);
		
		
	}
	
	int n;
	char buff[SERIAL_BUFF_SIZE];
	
	n = read(mainfd, buff, SERIAL_BUFF_SIZE);
	
	// if has 5 characters, is probably just " DSP:> "
	if ((configData.verboseMode) && (n != 0)) { ROS_WARN("read(): n = (%d)", n); }
	
	sleep(2); //required to make flush work, for some reason
	tcflush(mainfd,TCIOFLUSH);

	//ROS_WARN("Attempting initial reading of thermistor, 1 failure is expected..");
	// getThermistorReading();
	
	//sleep(2); //required to make flush work, for some reason
	//tcflush(mainfd,TCIOFLUSH);
	
	//sleep(2); //required to make flush work, for some reason
	// tcflush(mainfd,TCIOFLUSH);
	
	// Ensure shutter is open...
	
	//n = read(mainfd, buff, SERIAL_BUFF_SIZE);
	
	//sleep(2); //required to make flush work, for some reason
	//tcflush(mainfd,TCIOFLUSH);
	
	//getThermistorReading();
	
	//sleep(2); //required to make flush work, for some reason
	//tcflush(mainfd,TCIOFLUSH);
	
	// Ensure shutter is open...
	
	//n = read(mainfd, buff, SERIAL_BUFF_SIZE);
	
	if (configData.disableSkimming) {
		if (statusNum == 1) {
			ROS_ERROR("Camera must now be powercycled: SKIMACTIVE was not originally off.");
			return false;
		} else {
			ROS_WARN("Assuming camera has been powercycled since SKIMACTIVE was saved as off.");
			return true;
		}		
	} else {
		return true;
	}

}
#endif

void streamerNode::getRectification(){

    cv::Mat Q;
    cv::stereoRectify(globalExtrinsicsData.cameraMatrix0, globalExtrinsicsData.distCoeffs0, globalExtrinsicsData.cameraMatrix1, globalExtrinsicsData.distCoeffs1,
                                globalCameraInfo.cameraSize,
                                globalExtrinsicsData.R, globalExtrinsicsData.T,
                                globalExtrinsicsData.R0, globalExtrinsicsData.R1, globalExtrinsicsData.P0, globalExtrinsicsData.P1,
                                Q,
                                cv::CALIB_ZERO_DISPARITY, configData.alpha, globalCameraInfo.cameraSize, &globalExtrinsicsData.roi0, &globalExtrinsicsData.roi1);

}

void streamerNode::releaseDevice() {
	
	if (!deviceCreated) { return; }
	
	setValidity(false);
	
	if ((configData.inputDatatype == DATATYPE_8BIT) || (configData.inputDatatype == DATATYPE_MM)) {
		getVideoCapture()->release();
	} else if (configData.inputDatatype == DATATYPE_RAW) {
		
		#ifdef _BUILD_FOR_ROS_
		getMainVideoSource()->close_video_capture();
		#endif
	}
	
	deviceCreated = false;
}

#ifdef _BUILD_FOR_ROS_
void streamerNode::serverCallback(thermalvis::streamerConfig &config, uint32_t level) {
#else
void streamerNode::serverCallback(streamerConfig &config) {
#endif

	if (configData.verboseMode){ ROS_INFO("Processing reconfigure request..."); }

	configData.minTemperature = config.minTemperature;
	configData.maxTemperature = config.maxTemperature;

	configData.tempGrad = config.tempGrad;
	configData.tempIntercept = config.tempIntercept;
	
	if (configData.autoTemperature != config.autoTemperature) {
		lastMinDisplayTemp = -9e99, lastMaxDisplayTemp = 9e99;
		configData.autoTemperature = config.autoTemperature;
	}
	
	
	if (configData.detectorMode != config.detectorMode) {
		
		configData.detectorMode = config.detectorMode;
		
		if (!configData.serialComms) {
			ROS_WARN("Selecting a detector mode has no effect unless serial comms are enabled.");
		} else {
			updateDetectorMode = true;
		}
		
		
	}
	
	
	configData.usbMode = config.usbMode;
	if (!configData.serialComms) {
		if (configData.usbMode != config.usbMode) {
			ROS_WARN("Selecting a USB mode has no effect unless serial comms are enabled.");
		}
	} else {
		
		updateUSBMode = true;
		
		if (configData.usbMode == USB_MODE_16) {
			if (configData.verboseMode) { ROS_INFO("changing <configData.inputDatatype> to DATATYPE_RAW"); }
			config.inputDatatype = DATATYPE_RAW;
			if (strcmp(configData.outputTypeString.c_str(), "CV_8UC3")) { configData.outputTypeString = "CV_16UC1"; }
		} else if (configData.usbMode == USB_MODE_8) {
			config.inputDatatype = DATATYPE_8BIT;
			if (strcmp(configData.outputTypeString.c_str(), "CV_8UC3")) { configData.outputTypeString = "CV_8UC1"; }
		}
	
		
	}
	
	if (configData.serialPollingRate != config.serialPollingRate) {
		configData.serialPollingRate = config.serialPollingRate;
		if (configData.verboseMode) { ROS_INFO("Updating period with rate (%f)", configData.serialPollingRate); }
		
#ifdef _BUILD_FOR_ROS_
		if (configData.serialPollingRate > 0.1) {
			serial_timer.setPeriod(ros::Duration(1.0 / configData.serialPollingRate));
		} else {
			serial_timer.setPeriod(ros::Duration(1.0 / 1.0));
		}
	
#endif
	}

	if (configData.maxNucInterval != config.maxNucInterval) {
		
		configData.maxNucInterval = config.maxNucInterval;
		ROS_WARN("NUC interval changed by config...");
		updateNucInterval = true;
	}
	
	if (configData.maxNucThreshold != config.maxNucThreshold) {
		
		configData.maxNucThreshold = config.maxNucThreshold;
		updateNucInterval = true;
	}
	
	
	if (!configData.temporalSmoothing) {
		lastMedian = -1.0;
	}
	
	configData.verboseMode = config.verboseMode;
	
	configData.normFactor = config.normFactor;

	if (config.framerate != 0.0) {
		configData.pauseMode = false;
	}
		
	
	if (alphaChanged) {
		if (configData.verboseMode) { ROS_INFO("Updating map..."); }
		updateMap();
		if (configData.verboseMode) { ROS_INFO("Map updated."); }
	}
	
	fusionFactor = config.fusionFactor;
	
	//ROS_WARN("Progressing...");
            
	if (config.framerate < 0.0) {
		
		if (configData.pollMode) {
		
			ROS_WARN("Invalid framerate (%f) so switching to capture mode.", config.framerate);
			configData.framerate = DEFAULT_READ_RATE;
			configData.pollMode = false;
			configData.captureMode = true;
			
		} else if (configData.readMode) {
			
			ROS_WARN("Invalid framerate (%f) so defaulting to (%f).", config.framerate, DEFAULT_READ_RATE);
			configData.framerate = DEFAULT_READ_RATE;

		} else if (configData.loadMode) {
			
			ROS_WARN("Invalid framerate (%f) so defaulting to (%f).", config.framerate, DEFAULT_READ_RATE);
			configData.framerate = DEFAULT_READ_RATE;
			
		} else if (configData.resampleMode) {
			
			ROS_WARN("Invalid framerate (%f) so switching to subscribe mode.", config.framerate);
			configData.framerate = DEFAULT_READ_RATE;
			configData.resampleMode = false;
			configData.subscribeMode = true;
			
		}

#ifdef _BUILD_FOR_ROS_
		timer.setPeriod(ros::Duration(1.0 / DEFAULT_READ_RATE));
#endif
	} else {
		
		if (configData.captureMode) {
		
			ROS_INFO("Specified framerate (%f) so switching to poll mode.", config.framerate);
			configData.framerate = config.framerate;
			configData.captureMode = false;
			configData.pollMode = true;
			
		} else if (configData.subscribeMode) {
			
			ROS_INFO("Specified framerate (%f) so switching to resample mode.", config.framerate);
			configData.framerate = config.framerate;
			configData.subscribeMode = false;
			configData.resampleMode = true;
			
		}
		
		if (config.framerate > 0.0) {
#ifdef _BUILD_FOR_ROS_
			timer.setPeriod(ros::Duration(1.0 / config.framerate));
#endif
		} else {
			configData.pauseMode = true;
#ifdef _BUILD_FOR_ROS_
			timer.setPeriod(ros::Duration(1.0 / 1.0));
#endif
		}
		
		
	}
	
	if (config.inputDatatype != configData.inputDatatype) {
		if (configData.readMode) {
			ROS_WARN("You cannot change the bit-reading mode mid-stream. Consider restarting.");
		} else if ((configData.captureMode) || (configData.pollMode)) {
			if (configData.verboseMode) { ROS_INFO("Resetting device in order to change streaming mode to (%d)...", config.inputDatatype); }
			releaseDevice();
			if (configData.verboseMode) { ROS_INFO("device released..."); }
			configData.inputDatatype = config.inputDatatype;
			if (configData.verboseMode) { ROS_INFO("Changing to (%d)", configData.inputDatatype); }
			setupDevice();
			if (configData.verboseMode) { ROS_INFO("Set up done."); }
		} else {
			configData.inputDatatype = config.inputDatatype;
		}

	}
	
	//configData.filterMode = config.filterMode;
	//configData.filterParam = config.filterParam;
	
	//configData.maxReadAttempts = config.maxReadAttempts;
	
	configData.normMode = config.normMode;
	
	bool wantsToRefreshCameras = false;
	
	if (config.output16bit) {
		wantsToRefreshCameras = true;
	} else if (!config.output16bit) {
		wantsToRefreshCameras = true;
	}
	
	if (config.output8bit) {
		wantsToRefreshCameras = true;
	} else if (!config.output8bit) {
		wantsToRefreshCameras = true;
	}
	
	if (config.outputColor) {
		wantsToRefreshCameras = true;
	} else if (!config.outputColor) {
		wantsToRefreshCameras = true;
	}
	
#ifdef _BUILD_FOR_ROS_
	if (wantsToRefreshCameras) refreshCameraAdvertisements();
#endif
            
	getMapping(config.map, configData.extremes, configData.mapCode, configData.mapParam);
	colourMap.load_standard(configData.mapCode, configData.mapParam);
    
	if (configData.outputFolder == "outputFolder") {
		
		ROS_WARN("No valid output folder specified...");
		configData.writeImages = false;
		configData.dumpTimestamps = false;
	}
	
       //HGH
        //configData.wantsToRectify = config.rectifyImages;

        if (config.undistortImages) {
                //if (map1.rows == 0) {
                    //HGH  initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalCameraInfo.R, globalCameraInfo.newCamMat, globalCameraInfo.cameraSize, CV_32FC1, map1, map2);
                    //HGH
                    if (configData.addExtrinsics){
                        if (configData.rectifyImages){
                            if (configData.camera_number == 0){
                                cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalExtrinsicsData.R0, globalExtrinsicsData.P0, globalCameraInfo.cameraSize, CV_32FC1,  map1, map2);
                            }else if (configData.camera_number == 1){
                                cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalExtrinsicsData.R1, globalExtrinsicsData.P1, globalCameraInfo.cameraSize, CV_32FC1,  map1, map2);
                            }
                            //initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalCameraInfo.R, globalCameraInfo.newCamMat, globalCameraInfo.cameraSize, CV_16SC2/*CV_32FC1*/, map1, map2);
                        }else{
                            cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs,  cv::Mat(), globalCameraInfo.newCamMat, globalCameraInfo.cameraSize, CV_32FC1, map1, map2);
                        }
                    }else{
                        cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs,  cv::Mat(), globalCameraInfo.newCamMat, globalCameraInfo.cameraSize, CV_32FC1, map1, map2);
                    }
                  //  }
		}

        configData.undistortImages = config.undistortImages;
	
	
	if (!firstServerCallbackProcessed) {
		firstServerCallbackProcessed = true;

		if (configData.loadMode) {
			if (!processFolder()) {
				ROS_ERROR("Processing of folder failed...");
				configData.dataValid = false;
			}
		}

	}
	
	if (configData.verboseMode) { ROS_INFO("Reconfigure request complete..."); }
}


void getMapping(int mapIndex, bool extremes, int& mapCode, int& mapParam) {
	
	switch (mapIndex) {
	case 0:
		mapCode = GRAYSCALE;
		break;
	case 1:
		mapCode = CIECOMP;
		break;
	case 2:
		mapCode = BLACKBODY;
		break;
	case 3:
		mapCode = RAINBOW;
		break;
	case 4:
		mapCode = IRON;
		break;
	case 5:
		mapCode = BLUERED;
		break;
	case 6:
		mapCode = JET;
		break;
	case 7:
		mapCode = CIELUV;
		break;
	case 8:
		mapCode = ICEIRON;
		break;
	case 9:
		mapCode = ICEFIRE;
		break;
	case 10:
		mapCode = REPEATED;
		break;
	case 11:
		mapCode = HIGHLIGHTED;
		break;
	case 12:
		mapCode = GRAYSCALE;
		break;
	default:
		mapCode = CIELUV;
	}
		
	extremes ? mapParam = 0 : mapParam = 1;
	
}

bool streamerNode::setupDevice() {
	
	if (configData.verboseMode) { ROS_INFO("Entered setupDevice()..."); }
	
	if (deviceCreated) { return false; }
	
	//ROS_WARN("%d ; %s", configData.device_num, configData.capture_device.c_str());
	
	colourMat.release();
	
	if ((configData.inputDatatype == DATATYPE_8BIT) || (configData.inputDatatype == DATATYPE_MM)) {
		
		if (configData.verboseMode) { ROS_INFO("Setting up device in 8-bit/MM mode..."); }
		getVideoCapture()->open(configData.device_num);
	} else if (configData.inputDatatype == DATATYPE_RAW) {
		if (configData.verboseMode) { ROS_INFO("Setting up device in 16-bit mode..."); }
		
		#ifdef _BUILD_FOR_ROS_
		int deviceWidth, deviceHeight;
		getMainVideoSource()->setup_video_capture(configData.capture_device.c_str(), deviceWidth, deviceHeight, configData.verboseMode);
		
		// Now use this opportunity to test/correct?
		
		if ((!configData.imageDimensionsSpecified) && (!configData.intrinsicsProvided)) {
			// Use these values..
			ROS_INFO("Using image size learned from device initialization (%d, %d)", deviceWidth, deviceHeight);
			globalCameraInfo.imageSize.at<unsigned short>(0, 0) = deviceWidth;
			globalCameraInfo.imageSize.at<unsigned short>(0, 1) = deviceHeight;
			globalCameraInfo.cameraSize = cv::Size(globalCameraInfo.imageSize.at<unsigned short>(0, 0), globalCameraInfo.imageSize.at<unsigned short>(0, 1));
	
			overwriteCameraDims();
	
		} else {
			// Compare the values to what you are using, and produce a warning if necessary...
			
			if ((globalCameraInfo.cameraSize.width != deviceWidth) || (globalCameraInfo.cameraSize.height != deviceHeight)) {
				ROS_WARN("Device-based image size estimates (%d, %d) conflict with provided image size (%d, %d)", deviceWidth, deviceHeight, globalCameraInfo.cameraSize.width, globalCameraInfo.cameraSize.height);
			}
			
		}
		#endif
	}
	
	if (configData.verboseMode) { ROS_INFO("Device set up!"); }
	
	setValidity(true);
	deviceCreated = true;
	return true;
}

void streamerNode::updateMap() {

	if (globalCameraInfo.cameraMatrix.rows == 0) return;
	
	if (configData.verboseMode) { ROS_INFO("Updating map..."); }
	
	if (configData.autoAlpha) {
		if (configData.verboseMode) { ROS_INFO("(%d), (%d), (%d)", globalCameraInfo.cameraMatrix.rows, globalCameraInfo.distCoeffs.rows, globalCameraInfo.cameraSize.height); }
		if (configData.verboseMode) { ROS_INFO("globalCameraInfo.cameraSize = (%d, %d)", globalCameraInfo.cameraSize.width, globalCameraInfo.cameraSize.height); }
		configData.alpha = findBestAlpha(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalCameraInfo.cameraSize);

                if (configData.verboseMode) { ROS_INFO("Optimal alpha: (%f)", configData.alpha); }

	}

	if (configData.verboseMode) { ROS_INFO("Debug (%d)", 0); }
	
	cv::Rect* validPixROI = 0;
	globalCameraInfo.newCamMat = getOptimalNewCameraMatrix(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalCameraInfo.cameraSize, configData.alpha, globalCameraInfo.cameraSize, validPixROI, centerPrincipalPoint);
	
        //HGH initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalCameraInfo.R, globalCameraInfo.newCamMat, globalCameraInfo.cameraSize, CV_32FC1, map1, map2);
        //HGH
        if (configData.addExtrinsics){

            //update camera info extrinsics...
            getRectification();

#ifdef _BUILD_FOR_ROS_
            updateCameraInfoExtrinsics();
#endif
            if (configData.rectifyImages){
                if (configData.camera_number == 0){
                    cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalExtrinsicsData.R0, globalExtrinsicsData.P0, globalCameraInfo.cameraSize, CV_32FC1,  map1, map2);
                }else if (configData.camera_number == 1){
                    cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs, globalExtrinsicsData.R1, globalExtrinsicsData.P1, globalCameraInfo.cameraSize, CV_32FC1,  map1, map2);
                }
            }else{
                cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs,  cv::Mat(), globalCameraInfo.newCamMat, globalCameraInfo.cameraSize, CV_32FC1, map1, map2);
            }

        }else{
            cv::initUndistortRectifyMap(globalCameraInfo.cameraMatrix, globalCameraInfo.distCoeffs,  cv::Mat() , globalCameraInfo.newCamMat, globalCameraInfo.cameraSize, CV_32FC1, map1, map2);
        }

	alphaChanged = false;
	
	if (configData.verboseMode) { ROS_INFO("Map updated."); }
}

void streamerNode::act_on_image() {
	
#ifdef _BUILD_FOR_ROS_
	updateCameraInfo();
	newImage = cv::Mat(cv_ptr->image);
#else
	cv::Mat newImage(*bridgeReplacement);
#endif

	cv::Mat grayImage;
	
	if (newImage.type() == CV_16UC3) {
		cv::cvtColor(newImage, frame, CV_RGB2GRAY);
	} else frame = cv::Mat(newImage);
	
	readyToPublish = true;
	
	if (!configData.resampleMode) {
		if (processImage()) {
			if (readyToPublish) {
#ifdef _BUILD_FOR_ROS_
				updateCameraInfo();
#endif
				publishTopics();
				writeData();
			}
		}
	}
	
	return;
}

bool streamerNode::retrieveRawFrame() {
	if (configData.loadMode) {

		if (inputList.size() == 0) return false;

		if (frameCounter >= int(inputList.size())) {
			if (configData.loopMode) {
				frameCounter = 0;
			} else return false;
		}

		std::string full_path = std::string(configData.folder) + "/" + inputList.at(frameCounter);

		#ifdef _OPENCV_VERSION_3_PLUS_
		frame = cv::imread(full_path, cv::IMREAD_ANYDEPTH);
		#else
		frame = cv::imread(full_path, CV_LOAD_IMAGE_ANYDEPTH);
		#endif

		frameCounter++;
		return true;
	}
	return false;
}

bool streamerNode::get8bitImage(cv::Mat& img) { 
	if (_8bitMat.rows == 0) return false;
	img = _8bitMat; 
	return true;
}

bool streamerNode::run() {
#ifdef _BUILD_FOR_ROS_
	if ((configData.subscribeMode) || (configData.resampleMode)) return runBag();
#endif
	if (configData.readMode) return runRead();
	if (configData.loadMode) return runLoad();
	if ((configData.captureMode) || (configData.pollMode)) return runDevice();
	return false;
}

bool streamerNode::processImage() {
	
	if (frame.rows == 0) return false;
	
	if (configData.resizeImages) {
		resize(frame, rzMat, cv::Size(configData.desiredCols, configData.desiredRows));
		frame = cv::Mat(rzMat);
	}
	
	
	if (configData.removeDuplicates || configData.markDuplicates || configData.outputDuplicates) {
		if (matricesAreEqual(frame, lastFrame)) {
			
			#ifdef _BUILD_FOR_ROS_
			lastNucPerformed_at_the_earliest = ros::Time::now();		
			#endif

			if (configData.markDuplicates) lastIsDuplicate = true;
			
			if (configData.outputDuplicates) ofs_duplicates_log << "1" << endl;
			
			if (configData.removeDuplicates) {
				if (configData.loadMode) frameCounter++;
				return false;
			}
			
		} else {
			if (configData.markDuplicates) lastIsDuplicate = false;
			if (configData.outputDuplicates) ofs_duplicates_log << "0" << endl;
		}
		frame.copyTo(lastFrame);
	}

    if (configData.verboseMode){ ROS_INFO("Processing image (%d)...", frameCounter); }

	(pastMeanIndex >= (configData.temporalMemory-1)) ? pastMeanIndex = 0 : pastMeanIndex++;
	
	if (configData.inputDatatype == DATATYPE_RAW) {
		
		_16bitMat = cv::Mat(frame);
		
		if (configData.normFactor > 0.0) {
			double percentile_levels[2];
			percentile_levels[0] = (configData.normFactor / 2.0);
			percentile_levels[1] = 1.0 - (configData.normFactor / 2.0);
			double percentile_values[2];
			findPercentiles(_16bitMat, percentile_values, percentile_levels, 2);
			thresholdRawImage(_16bitMat, percentile_values);
			
		}

		if (canRadiometricallyCorrect && configData.radiometricCorrection) {
			if (configData.radiometricBias != 0) _16bitMat += configData.radiometricBias;
			radMapper.apply(_16bitMat, temperatureMat, lastThermistorReading, configData.radiometricInterpolation);
		}

		if ((configData.outputColor) || (configData.output8bit) || ((configData.writeImages) && ((configData.outputType == OUTPUT_TYPE_CV_8UC3) || (configData.outputType == OUTPUT_TYPE_CV_8UC1)) )) {
			
			if (configData.verboseMode) { ROS_INFO("Entering here x123..."); }
			
			if ((canRadiometricallyCorrect && configData.radiometricCorrection) || configData.alreadyCorrected) {
				if (configData.verboseMode) { ROS_INFO("Entering here x124..."); }
				
				if (configData.alreadyCorrected) convertToTemperatureMat(_16bitMat, temperatureMat, configData.tempGrad, configData.tempIntercept);
				
				if (configData.autoTemperature) {
					
					if (configData.verboseMode) { ROS_INFO("Entering here x125..."); }
					
					if (configData.verboseMode) {
						double currMin, currMax;
						minMaxLoc(temperatureMat, &currMin, &currMax);
						ROS_WARN("Current temp image limits = (%f) : (%f, %f)", abs(currMax-currMin), currMin, currMax);
					}
					
					double currMin, currMax, currRange, newMin, newMax;
					minMaxLoc(temperatureMat, &currMin, &currMax);
					
					if (configData.stepChangeTempScale) {
						
						currRange = currMax - currMin;
					
						if (currRange < 20.0) {
							newMin = floor(currMin);
							newMax = ceil(currMax);
						} else if (currRange < 50.0) {
							newMin = 2*floor(currMin/2);
							newMax = 2*ceil(currMax/2);
						} else if (currRange < 100.0) {
							newMin = 5*floor(currMin/5);
							newMax = 5*ceil(currMax/5);
						} else if (currRange < 200.0) { 
							newMin = 10*floor(currMin/5);
							newMax = 10*ceil(currMax/5);
						} else if (currRange < 500.0) {
							newMin = 20*floor(currMin/5);
							newMax = 20*ceil(currMax/5);
						} else if (currRange < 1000.0) {
							newMin = 50*floor(currMin/5);
							newMax = 50*ceil(currMax/5);
						} else { 
							newMin = 100*floor(currMin/5);
							newMax = 100*ceil(currMax/5);
						}
						
						if ( (abs(newMin - lastMinDisplayTemp) > 0.1*currRange) || (abs(newMax - lastMaxDisplayTemp) > 0.1*currRange) ) {
							// So if the change in min or max is more than 30% of the current range, then update..
							
							ROS_WARN("MIN/MAX for display changed to = (%f, %f)", newMin, newMax);
							lastMinDisplayTemp = newMin;
							lastMaxDisplayTemp = newMax;
						}
						
					} else {
						lastMinDisplayTemp = currMin;
						lastMaxDisplayTemp = currMax;
					}
					
					temperatureDownsample(temperatureMat, preFilteredMat, lastMinDisplayTemp, lastMaxDisplayTemp);
					
				} else {
					if (configData.verboseMode) { ROS_INFO("Downsampling with (%f, %f)", configData.minTemperature, configData.maxTemperature); }
					temperatureDownsample(temperatureMat, preFilteredMat, configData.minTemperature, configData.maxTemperature);
				}

			} else {
				if (configData.verboseMode) { ROS_INFO("Entering here x126..."); }
				adaptiveDownsample(_16bitMat, preFilteredMat, configData.normMode, configData.normFactor); //, configData.filterMode);
			}
		} 
		
		if (configData.output16bit || (configData.writeImages && (configData.outputType == OUTPUT_TYPE_CV_16UC1))) {
			(canRadiometricallyCorrect && configData.radiometricCorrection && configData.radiometricRaw) ? temperatureDownsample16(temperatureMat, scaled16Mat) : scaled16Mat = _16bitMat;
		}
		
	} else if (configData.inputDatatype == DATATYPE_8BIT) {
		
		if (frame.channels() == 3) {
			
			if (firstFrame) {
				isActuallyGray = checkIfActuallyGray(frame);
				firstFrame = false;
				
				if (configData.forceInputGray) {
					isActuallyGray = true;
					if (configData.verboseMode) { ROS_INFO("Forcing input images to be considered gray."); }
				}
			}
			
		}
		
		if ((frame.channels() == 1) || (isActuallyGray)) {
			
			if (frame.channels() == 1) {
				workingFrame = cv::Mat(frame);
			} else if (isActuallyGray) {
				if (((configData.outputColor) || (configData.outputType == OUTPUT_TYPE_CV_8UC3)) || ((configData.output8bit) || (configData.outputType == OUTPUT_TYPE_CV_8UC1))) {
					cvtColor(frame, workingFrame, CV_RGB2GRAY);
				}
			}

			// Need to normalize if appropriate
			process8bitImage(workingFrame, preFilteredMat, configData.normMode, configData.normFactor);
			
		} else if (frame.channels() == 3) {
			colourMat = cv::Mat(frame);
			if ((configData.output8bit) || (configData.outputType == OUTPUT_TYPE_CV_8UC1)) cvtColor(colourMat, preFilteredMat, CV_RGB2GRAY);
		}

	} else if (configData.inputDatatype == DATATYPE_MM) {
		
		if (frame.channels() != 3) {
			ROS_ERROR("Frames must have 3 channels to be used for multi-modal fusion.");
			if (firstFrame) {
				isActuallyGray = false;
				firstFrame = false;
			}
		} else {
			//ROS_ERROR("Processing 3-channel image as an MM...");
			// This is where you'll break the image apart and perform fusion...
			cv::Mat thermal, visible;
			splitMultimodalImage(frame, thermal, visible);
			
			double fusion_params[2];
			fusion_params[0] = max(min(0.5, 0.5 - (fusionFactor/2.0)), 0.0);
			fusion_params[1] = max(min(0.5, 1.0 - (fusionFactor/2.0)), 1.0);

			colourMap.fuse_image(thermal, visible, colourMat, fusion_params);

			if ((configData.output8bit) || (configData.outputType == OUTPUT_TYPE_CV_8UC1)) cvtColor(colourMat, preFilteredMat, CV_RGB2GRAY);
		}
	}
	
	
	(configData.filterMode > 0) ? applyFilter(preFilteredMat, smoothedMat, configData.filterMode, configData.filterParam) : smoothedMat = preFilteredMat;
	
	if (configData.temporalSmoothing) {
		if (((configData.outputColor) || ((configData.writeImages) && (configData.outputType == OUTPUT_TYPE_CV_8UC3))) || ((configData.output8bit) || ((configData.writeImages) && (configData.outputType == OUTPUT_TYPE_CV_8UC1)))) {
			// If you want to output any kind of 8-bit format...
			
			cv::Scalar means = mean(smoothedMat);
			pastMeans[pastMeanIndex] = means[0];

			double temporalMean = 0.0;
			
			for (int iii = 0; iii < min(frameCounter+1, configData.temporalMemory); iii++) temporalMean += pastMeans[iii];
			
			temporalMean /= min(frameCounter+1, configData.temporalMemory);
			shiftDiff = (((temporalMean - pastMeans[pastMeanIndex]) > 0.0) ? 1.0 : (((temporalMean - pastMeans[pastMeanIndex]) < 0.0) ? -1.0 : 0.0)) * min(abs((pastMeans[pastMeanIndex] - temporalMean)), configData.maxIntensityChange);
			_8bitMat = smoothedMat + shiftDiff;
		}
		
	} else _8bitMat = smoothedMat;

	if ((configData.inputDatatype != DATATYPE_MM) && ((configData.inputDatatype != DATATYPE_8BIT) || (frame.channels() != 3) || isActuallyGray)) {
	
		if ((configData.outputColor) || ((configData.writeImages) && (configData.outputType == OUTPUT_TYPE_CV_8UC3))) colourMap.falsify_image(_8bitMat, colourMat, configData.mapParam);
	}

	if (configData.displayThermistor) { 
		
		// IMPLEMENTING HYSTERESIS
		double displayVal;
		
		// Determine if higher than last displayed..
		if (newThermistorReading > lastDisplayed) {
			// Increasing...
			displayVal = newThermistorReading-0.015;
			//ROS_INFO("Increasing from (%f) to (%f), last displayed was (%f), now displaying (%f)", lastThermistorReading, newThermistorReading, lastDisplayed, displayVal);
		} else {
			// Decreasing...
			displayVal = newThermistorReading+0.015;
			//ROS_INFO("Decreasing from (%f) to (%f), last displayed was (%f), now displaying (%f)", lastThermistorReading, newThermistorReading, lastDisplayed, displayVal);
		}
		
		displayVal = round(20.0*displayVal)/20.0;
		ROS_INFO("Temperature = (%.2f)", displayVal); 
		lastDisplayed = displayVal;
	}
	
	frameCounter++;
	if (!readyToPublish) readyToPublish = true;
	
	return true;
}

void streamerNode::publishTopics() {

#ifdef _BUILD_FOR_ROS_
	initializeMessages();
#endif

	bool cameraPublished = false;

#ifdef _BUILD_FOR_ROS_
	if (configData.wantsToDumpTimestamps) ofs << camera_info.header.stamp.toNSec() << endl;
#endif

	if ((configData.output16bit) || (configData.writeImages &&  (configData.outputType == OUTPUT_TYPE_CV_16UC1))) {

		if (configData.undistortImages) {
			if (scaled16Mat.data == _16bitMat_pub.data) _16bitMat_pub = cv::Mat(scaled16Mat.size(), scaled16Mat.type());
			remap(scaled16Mat, _16bitMat_pub, map1, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
		} else _16bitMat_pub = cv::Mat(scaled16Mat);

#ifdef _BUILD_FOR_ROS_		
			if (configData.output16bit) {
				std::copy(&(_16bitMat_pub.at<char>(0,0)), &(_16bitMat_pub.at<char>(0,0))+(_16bitMat_pub.cols*_16bitMat_pub.rows*2), msg_16bit.data.begin());
				if (!cameraPublished) {
					pub_16bit.publish(msg_16bit, camera_info);
					cameraPublished = true;
				} else pub_16bit_im.publish(msg_16bit);

				//HGH
				if (configData.republishSource==REPUBLISH_CODE_16BIT) {
					(configData.republishNewTimeStamp) ? pub_republish.publish(msg_16bit, camera_info, ros::Time::now()) : pub_republish.publish(msg_16bit, camera_info);
				}
			}
#endif	

	}

	if ((configData.output8bit) || (configData.writeImages &&  (configData.outputType == OUTPUT_TYPE_CV_8UC1))) {

		if (configData.undistortImages) {
			if (_8bitMat.data == _8bitMat_pub.data) _8bitMat_pub = cv::Mat(_8bitMat.size(), _8bitMat.type());
			remap(_8bitMat, _8bitMat_pub, map1, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
		} else {
			_8bitMat_pub = cv::Mat(_8bitMat);
		}

#ifdef _BUILD_FOR_ROS_	
		if (configData.output8bit) {

			//HGH
			if (configData.drawReticle) {
				line(_8bitMat_pub, cv::Point(0,_8bitMat_pub.rows/2), cv::Point(_8bitMat_pub.cols, _8bitMat_pub.rows/2), cv::Scalar(0),1,8);
				line(_8bitMat_pub, cv::Point(_8bitMat_pub.cols/2,0), cv::Point(_8bitMat_pub.cols/2, _8bitMat_pub.rows), cv::Scalar(0),1,8);
			}

			std::copy(&(_8bitMat_pub.at<unsigned char>(0,0)), &(_8bitMat_pub.at<unsigned char>(0,0))+(_8bitMat_pub.cols*_8bitMat_pub.rows), msg_8bit.data.begin());

			if (!cameraPublished) {
				pub_8bit.publish(msg_8bit, camera_info);
				cameraPublished = true;
			} else pub_8bit_im.publish(msg_8bit);

			//HGH
			if (configData.republishSource==REPUBLISH_CODE_8BIT_MONO) {

				if (configData.republishNewTimeStamp) {
					//republish with new time stamps
					pub_republish.publish(msg_8bit, camera_info, ros::Time::now());
				} else pub_republish.publish(msg_8bit, camera_info);
			}

		}

#endif
	
	}

	if ((configData.outputColor) || (configData.writeImages &&  (configData.outputType == OUTPUT_TYPE_CV_8UC3))) {

		if (colourMat.rows > 0) {

			if (configData.undistortImages) {
				if (colourMat.data == colourMat_pub.data)colourMat_pub = cv::Mat(colourMat.size(), colourMat.type());
				remap(colourMat, colourMat_pub, map1, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
			} else colourMat_pub = cv::Mat(colourMat);

#ifdef _BUILD_FOR_ROS_	
			if (configData.outputColor) {
				if (configData.drawReticle){
					line(colourMat_pub, cv::Point(0,colourMat_pub.rows/2), cv::Point(colourMat_pub.cols, colourMat_pub.rows/2), cv::Scalar(0,255,0),1,8);
					line(colourMat_pub, cv::Point(colourMat_pub.cols/2,0), cv::Point(colourMat_pub.cols/2, colourMat_pub.rows), cv::Scalar(0,255,0),1,8);         
				}

				std::copy(&(colourMat_pub.at<cv::Vec3b>(0,0)[0]), &(colourMat_pub.at<cv::Vec3b>(0,0)[0])+(colourMat_pub.cols*colourMat_pub.rows*3), msg_color.data.begin());

				if (!cameraPublished) {
					if (configData.verboseMode) { ROS_INFO("%s << Publishing the whole camera...", __FUNCTION__); }
					pub_color.publish(msg_color, camera_info);
					cameraPublished = true;
				} else pub_color_im.publish(msg_color);

				if (configData.republishSource == REPUBLISH_CODE_8BIT_COL){
					(configData.republishNewTimeStamp) ? pub_republish.publish(msg_color, camera_info, ros::Time::now()) : pub_republish.publish(msg_color, camera_info);
				}
			}
#endif
		}
	}

	readyToPublish = false;

}

bool streamerNode::runDevice() {
	
	if (configData.captureMode) {
		ROS_INFO("Video stream (device: %d) started...", configData.device_num);
	} else if (configData.pollMode) {
		ROS_INFO("Video polling (device: %d) started...", configData.device_num);
	}
	
	setupDevice();
	
	while (isVideoValid()) {
		
		

		if (configData.verboseMode){ ROS_INFO("Starting loop.."); }
		if (configData.captureMode) {
			
			#ifdef _BUILD_FOR_ROS_
			if (configData.verboseMode){ ROS_INFO("About to spin"); }
			ros::spinOnce();
			if (configData.verboseMode){ ROS_INFO("Spun"); }
			#endif
			
			if (streamCallback()) {
				
				
				
				if (configData.verboseMode){ ROS_INFO("About to process"); }
				
				
				if (processImage()) {
				
					if (configData.verboseMode){ ROS_INFO("Processed image."); }
					
					if (readyToPublish && (!configData.serialComms || !updateUSBMode)) {
						if (configData.verboseMode){ ROS_INFO("About to publish topics..."); }
						publishTopics();		
						if (configData.verboseMode){ ROS_INFO("Topics published."); }
						writeData();
						if (configData.verboseMode){ ROS_INFO("Data written."); }
					}
				}
			}
			
			
			
		} else if (configData.pollMode) {
			
			// Want to keep on calling streamCallback until the time is right to capture the frame
			streamCallback(false);
			#ifdef _BUILD_FOR_ROS_
			ros::spinOnce();
			#endif
		}
		
		if (configData.verboseMode){ ROS_INFO("Ending loop."); }
	}
	
	ROS_INFO("About to release device here...");
	releaseDevice();
	
	if (configData.captureMode) {
		ROS_INFO("Video capture terminating...");
	} else if (configData.pollMode) {
		ROS_INFO("Video polling terminating...");
	}
	
	return true;
}

bool streamerNode::runLoad() {
	ROS_INFO("Image load started...");
	
	if (!processFolder()) {
		ROS_ERROR("Processing of folder failed...");
		return false;
	}
	
	do {
		
		frameCounter = 0;
		setValidity(true);
		
		while (isVideoValid()) {
			#ifdef _BUILD_FOR_ROS_
			ros::spinOnce();		
			#endif
		}
		
	} while (configData.loopMode && !wantsToShutdown());
	
	return true;
	
}

bool streamerNode::streamCallback(bool capture) {
	
	int currAttempts = 0;
	
	#ifdef _BUILD_FOR_ROS_
	if (configData.verboseMode) { 
		ros::Time callbackTime = ros::Time::now();
		ROS_INFO("Entered <streamCallback> at (%f)", callbackTime.toSec());
	}
	#endif

	if ((configData.inputDatatype == DATATYPE_8BIT) || (configData.inputDatatype == DATATYPE_MM)) {
		
		#ifdef _BUILD_FOR_ROS_
		if (configData.verboseMode){ ROS_INFO("Updating camera info..."); }
		updateCameraInfo();
		if (configData.verboseMode){ ROS_INFO("Camera info updated."); }
		ofs_call_log << ros::Time::now().toNSec() << endl;
		#endif

		if (configData.verboseMode){ ROS_INFO("Capturing frame (8-bit/MM)..."); }
		cap >> frame;
		
		if (configData.fixDudPixels) {
			if (configData.verboseMode){ ROS_INFO("Fixing dud pixels..."); }
			fix_bottom_right(frame);
			if (configData.verboseMode){ ROS_INFO("Dud pixels fixed."); }
		}
		
		if (configData.verboseMode){ ROS_INFO("Frame captured."); }
		#ifdef _BUILD_FOR_ROS_
		ofs_retrieve_log << ros::Time::now().toNSec() << endl;
		#endif
	} else if (configData.inputDatatype == DATATYPE_RAW) {
		
		#ifdef _BUILD_FOR_ROS_
		ros::Time callTime, retrieveTime;
		
		callTime = ros::Time::now();
		if (configData.verboseMode) { ROS_INFO("Capturing frame (16-bit)... (%f)", callTime.toSec()); }
		#endif

		//bool frameRead = false;
		while ((configData.maxReadAttempts == 0) || (currAttempts < configData.maxReadAttempts)) {
			// Keep on looping, but if max defined, only until curr attempts is high enough
			
			if (configData.verboseMode){ ROS_INFO("Attempting to capture frame..."); }
			
			#ifndef _WIN32
			if (av_read_frame(mainVideoSource->pIFormatCtx, &(mainVideoSource->oPacket)) != 0) {
				if (configData.verboseMode){ ROS_WARN("av_read_frame() failed."); }
				currAttempts++;
				continue;
			}
			#endif
			
			if (configData.verboseMode){ ROS_INFO("Frame captured successfully."); }
			
			#ifdef _BUILD_FOR_ROS_
			retrieveTime = ros::Time::now();
			firmwareTime = mainVideoSource->oPacket.pts;
			

			if (configData.verboseMode){ ROS_INFO("Updating camera info..."); }
			updateCameraInfo();
			if (configData.verboseMode){ ROS_INFO("Camera info updated."); }
			
			if (mainVideoSource->bRet < 0) {
				if (configData.verboseMode) { ROS_WARN("(mainVideoSource->bRet < 0) failed."); }
				currAttempts++;
				continue;
			}
			
			if (mainVideoSource->oPacket.stream_index != mainVideoSource->ixInputStream) {
				if (configData.verboseMode) { ROS_WARN("(mainVideoSource->oPacket.stream_index != mainVideoSource->ixInputStream) failed."); }
				currAttempts++;
				continue;
			}
			if (configData.verboseMode){ ROS_INFO("Decoding frame..."); }
			avcodec_decode_video2(mainVideoSource->pICodecCtx, mainVideoSource->pFrame, &(mainVideoSource->fFrame),&(mainVideoSource->oPacket));
			if (configData.verboseMode){ ROS_INFO("Frame decoded."); }
			
			av_free_packet(&(mainVideoSource->oPacket));
			if (configData.verboseMode){ ROS_INFO("Frame freed."); }
			
			
			if (!(mainVideoSource->fFrame)) {
				if (configData.verboseMode) { ROS_WARN("(!(mainVideoSource->fFrame)) failed."); }
				currAttempts++;
				continue;
			}
			
			if (capture) {
				if (configData.verboseMode){ ROS_INFO("Accepting image..."); }
				acceptImage((void*) *(mainVideoSource->pFrame->data));
				if (configData.verboseMode){ ROS_INFO("Image accepted."); }
				
				char outbuff[256];
				
				uint32_t internal_sec, internal_nsec;
		
				internal_sec = firmwareTime / 1000000;
				internal_nsec = (firmwareTime % 1000000) * 1000;
				
				//sprintf(outbuff, "%016d000", firmwareTime);
				sprintf(outbuff, "%010d.%09d", internal_sec, internal_nsec);
				
				if (configData.wantsToDumpTimestamps) {
					ofs_internal_log << outbuff << endl;
					//ofs_retrieve_log << retrieveTime.toNSec() << endl;
					//ofs_retrieve_log << retrieveTime.sec << "." << retrieveTime.nsec << endl;
					char output_time[256];
					sprintf(output_time,"%010d.%09d", retrieveTime.sec, retrieveTime.nsec);
					ofs_retrieve_log << output_time << endl;
					ofs_call_log << callTime.toNSec() << endl;
				}
				
			}
			#endif

			// Want to exit loop if it actually worked
			if (configData.verboseMode){ ROS_INFO("Frame read."); }
			break;
			
		}

		if ((currAttempts >= configData.maxReadAttempts) && (configData.maxReadAttempts != 0))  {
			ROS_ERROR("Frame reading failed after (%d) attempts.", currAttempts);
			setValidity(false);
			return false;
		}
		
		// If it got here, it means it succeeded so one attempt was not recorded...
		if (currAttempts >= 1) {
			ROS_WARN("Frame reading required (%d) attempts.", currAttempts+1);
		}
	}
	
	// processImage();
	
	//ROS_INFO("Exiting callback.");
	if (configData.verboseMode){ ROS_INFO("Exiting callback..."); }
	
	return true;
}

bool streamerNode::processFolder() {
	
#ifndef _WIN32
	DIR * dirp;
	struct dirent * entry;
	
	dirp = opendir(configData.folder.c_str());
	
	if (dirp == NULL) {
		ROS_ERROR("Opening of directory (%s) failed.", configData.folder.c_str());
		return false;
	}

	while ((entry = readdir(dirp)) != NULL) {
		if (entry->d_type == DT_REG) { // If the entry is a regular file
			inputList.push_back(string(entry->d_name));
			fileCount++;
		}
	}
	closedir(dirp);
#else
	std::string full_dir = configData.folder + "/";
	boost::filesystem::path someDir(full_dir);
		
	if ( boost::filesystem::exists(someDir) && boost::filesystem::is_directory(someDir)) {
		boost::filesystem::directory_iterator end_iter;	
		for( boost::filesystem::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter) {
			if (boost::filesystem::is_regular_file(dir_iter->status()) ) {

				std::stringstream temp;
				temp << dir_iter->path().filename();
				string name;
				name = temp.str();
				boost::replace_all(name, "\"", "");

				if ((name == ".") || (name == "..") || (name[0] == '.') || (name.size() < 5)) continue;

				if (name.size() > 5) {
					if ((name[name.size()-4] != '.') && (name[name.size()-5] != '.')) continue;
				} else if (name[name.size()-4] != '.') {
					continue;
				}

				inputList.push_back(name);
				
			}
		}
		

	} else {
		return false;
	}

	return true;


	return false;
#endif
	
	sort(inputList.begin(), inputList.end());

	if(fileCount == -1)	{
		ROS_ERROR("File counting error.\n");
		return false;
	}

	ROS_INFO("No. of images in folder = %d\n", fileCount);

	if (fileCount == 0) {
		ROS_ERROR("Returning, because no images are in folder.\n");
		return false;
	}
	
	return true;
}

void streamerNode::writeData() {
	
	if (configData.writeImages) {	
		if ((frameCounter-1) != lastWritten) {

			char *outputFilename;
			outputFilename = (char*) malloc(256);
			
			if (configData.loadMode && configData.keepOriginalNames) {
				size_t findDot = inputList.at(frameCounter-1).rfind(".");
				string partialName;
				partialName = inputList.at(frameCounter-1).substr(0, findDot);
				sprintf(outputFilename, "%s/%s.%s", configData.outputFolder.c_str(), partialName.c_str(), configData.outputFormatString.c_str());
			} else sprintf(outputFilename, "%s/frame%06d.%s", configData.outputFolder.c_str(), frameCounter-1, configData.outputFormatString.c_str());
			
			if (configData.verboseMode) { ROS_INFO("Output name = (%s), outputType = (%d)", outputFilename, configData.outputType); }
			
			if (configData.outputType == OUTPUT_TYPE_CV_16UC1) {
				if (scaled16Mat.rows > 0) {
					
					if (configData.outputFormatString == "png") {
						imwrite(outputFilename, _16bitMat_pub, configData.outputFileParams);
					} else if ((configData.outputFormatString == "pgm") || (configData.outputFormatString == "ppm")) {
						imwrite(outputFilename, _16bitMat_pub);
					}
				}
			} else if (configData.outputType == OUTPUT_TYPE_CV_8UC3) {
				if (colourMat.rows > 0) {
					
					//ROS_ERROR("Actually writing...");
					
					if ((configData.outputFormatString == "png") || (configData.outputFormatString == "jpg")) {
						imwrite(outputFilename, colourMat_pub, configData.outputFileParams);
					} else if ((configData.outputFormatString == "bmp") || (configData.outputFormatString == "ppm")) {
						imwrite(outputFilename, colourMat_pub);
					}
				}
			} else if (configData.outputType == OUTPUT_TYPE_CV_8UC1) {
				if (_8bitMat.rows > 0) {
					if ((configData.outputFormatString == "png") || (configData.outputFormatString == "jpg")) {
						imwrite(outputFilename, _8bitMat_pub, configData.outputFileParams);
					} else if ((configData.outputFormatString == "bmp") || (configData.outputFormatString == "pgm") || (configData.outputFormatString == "ppm")) {
						imwrite(outputFilename, _8bitMat_pub);
					}
				}
			}			
			lastWritten = frameCounter - 1;
		}
	}
	
	if (configData.writeVideo) {
		if (!videoInitialized) {
			if (configData.videoType == "CV_16UC1") {
				// 0 writes uncompressed, 1 gives user option
				vid_writer.open(configData.outputVideo, CV_FOURCC('P','I','M','1'), ((int) configData.framerate), scaled16Mat.size(), true);
			} else if (configData.videoType == "CV_8UC3") {
				vid_writer.open(configData.outputVideo, CV_FOURCC('P','I','M','1'), ((int) configData.framerate), colourMat.size()); // , true);
			} else if (configData.videoType == "CV_8UC1") {
				vid_writer.open(configData.outputVideo, CV_FOURCC('X', 'V', 'I', 'D'), ((int) configData.framerate), _8bitMat.size(), false);
			}
			
			videoInitialized = true;
		}
		
		if (configData.videoType == "CV_16UC1") {
			vid_writer << scaled16Mat;
		} else if (configData.videoType == "CV_8UC3") {
			vid_writer << colourMat;
		} else if (configData.videoType == "CV_8UC1") {
			vid_writer << _8bitMat;
		}

	}
}

bool streamerNode::runRead() {
	
	ROS_INFO("Video reading started...");	
	do {
		setValidity(true);
		if (configData.verboseMode) { ROS_INFO("Opening file (%s)...", configData.filename.c_str()); }
		if (configData.inputDatatype == DATATYPE_RAW) {
#ifndef _WIN32
			if (getMainVideoSource()->setup_video_file(configData.filename) < 0) {
				ROS_ERROR("Source configuration failed.");
				return false;
			}
#else
			return false;
#endif
		} else if ((configData.inputDatatype == DATATYPE_8BIT) || (configData.inputDatatype == DATATYPE_MM)) {
			getVideoCapture()->open(configData.filename.c_str());
			
			if(!cap.isOpened()) { // check if we succeeded
				ROS_ERROR("File open failed (using OpenCV).");
				return false;
			}
			//capture = cvCaptureFromAVI(configData.filename.c_str());
		}	
		
		if (configData.verboseMode) { ROS_INFO("Source configured."); }
		
		while (isVideoValid()) {
			#ifdef _BUILD_FOR_ROS_
			ros::spinOnce();
			#endif
		}
		
		if (configData.verboseMode) { ROS_INFO("Video complete."); }
		
		if (configData.inputDatatype == DATATYPE_RAW) {
			#ifdef _BUILD_FOR_ROS_
			getMainVideoSource()->close_video_file(configData.filename);
			#endif
		} else if ((configData.inputDatatype == DATATYPE_8BIT) || (configData.inputDatatype == DATATYPE_MM)) {
			getVideoCapture()->release();
		}

	} while (configData.loopMode && !wantsToShutdown());
	
	if (configData.verboseMode) { ROS_INFO("Video reading terminating..."); }
	
	return true;
	
}

	///brief	Initial receipt of an image. 
#ifdef _BUILD_FOR_ROS_
	void streamerNode::handle_camera(const sensor_msgs::ImageConstPtr& msg_ptr, const sensor_msgs::CameraInfoConstPtr& info_msg) {
#else
	void streamerNode::handle_camera(const cv::Mat& inputImage, const cameraInfoStruct *info_msg) {
#endif

	if (configData.syncMode != SYNCMODE_HARD) return;
	
	if ((!configData.subscribeMode) && (!configData.resampleMode)) return;
	
	if (configData.verboseMode) { ROS_INFO("Copying camera info over..."); }
	
#ifdef _BUILD_FOR_ROS_
	original_camera_info = *info_msg;
	if (configData.verboseMode) { ROS_INFO("original_camera_info.header.seq = (%d)", original_camera_info.header.seq); }
	original_time = info_msg->header.stamp;
	memcpy(&lastThermistorReading, &info_msg->binning_x, sizeof(float));

	
	if (configData.inputDatatype == DATATYPE_DEPTH) {
		cv_ptr = cv_bridge::toCvCopy(msg_ptr, "16UC1");
	} else {
		cv_ptr = cv_bridge::toCvCopy(msg_ptr, enc::BGR8);					// For some reason it reads as BGR, not gray
	}
#else
	bridgeReplacement = &inputImage;
#endif
	
	act_on_image();
	
}