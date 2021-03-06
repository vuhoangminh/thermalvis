/*! \file	keyframes.hpp
 *  \brief	Declarations for calculations and management relating to keyframes (for loop closure).
*/

#ifndef _THERMALVIS_KEYFRAMES_H_
#define _THERMALVIS_KEYFRAMES_H_

#include "general_resources.hpp"
#include "opencv_resources.hpp"

#include "features.hpp"

#define 	KF_CONNECTION_WEAK			0
#define 	KF_CONNECTION_GEOMETRIC		1
#define 	KF_CONNECTION_HOMOGRAPHIC	2

#define 	KF_TYPE_FIRST				0
#define 	KF_TYPE_LAST				1
#define 	KF_TYPE_GEOMETRIC			2
#define		KF_TYPE_HOMOGRAPHIC			3
#define		KF_TYPE_EXHAUSTED			4
#define		KF_TYPE_WEAK				5

#define		MAXIMUM_FEATURES_PER_FRAME	200

#define		MATCHING_SIZE_CONSTRAINT	0.2
#define		MATCHING_DIST_CONSTRAINT	100

/// \brief		Contains feature information corresponding to a keyframe which may eventually be used for loop closure
struct keyframe {
	
	unsigned int idx;
	cv::Mat im;
	vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;
	//Mat pose;
	bool poseDetermined;
	
	/// \brief      Uses an established keypoint detector to detect keypoints in the keyframe
	void detectKeypoints(cv::Ptr<cv::FeatureDetector>& detector);
	
	/// \brief      Uses an established descriptor extractor to extract descriptors in the keyframe
	void extractDescriptors(cv::Ptr<cv::DescriptorExtractor>& extractor);
	
};

/// \brief		Describes a link between two keyframes in terms of matched features
struct connection {
	
	unsigned int idx1, idx2;
	cv::Mat matchingMatrix;
	int type;
	double confidence;
	cv::Mat relation;
	bool processed;
	vector<vector<cv::DMatch> > matches1to2;
	
};

/// \brief		Stores information linking keyframes within a SLAM sequence
struct keyframeStore {
	
	vector<keyframe> keyframes;
	vector<connection> connections;
	unsigned int count;
	
	// Should also store matches
	
	keyframeStore();
	
	/// \brief      Finds the best pair of keyframes to initialize structure with
	bool getBestPair(int& idx1, int& idx2);
	
	/// \brief      Adds a keyframe to the store
	void addKeyframe(int idx, cv::Mat& image);
	
	/// \brief      Adds a connection between keyframes to the store
	void addConnection(int idx1, int idx2, int type = KF_CONNECTION_WEAK, cv::Mat rel = cv::Mat());
	
	/// \brief      Returns indices of connections that are strongly linked to this keyframe
	void findStrongConnections(int idx, vector<unsigned int>& cIndices);
	
	/// \brief      Finds all matches between weak keyframes
	void findMatches();
	
};

bool getValidLocalMaxima(cv::Mat& scores, unsigned int last_iii, unsigned int last_jjj, unsigned int& opt_iii, unsigned int& opt_jjj);

#endif
