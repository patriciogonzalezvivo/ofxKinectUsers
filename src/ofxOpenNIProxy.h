#ifndef OFXOPENNI
#define OFXOPENNI

//#define HARDWARE
//#define IMAGE

#define	MAX_USERS 8

// Open NI
#include "ofxOpenNIContext.h"
#include "ofxDepthGenerator.h"
#include "ofxUserGenerator.h"

#ifdef HARDWARE
#include "ofxHardwareDriver.h"
#endif

#ifdef IMAGE
#include "ofxImageGenerator.h"
#endif

// OpenFramework
#include "ofMain.h"

#include "ofxUser.h"
#include "ofxKTools.h"

//------------------------------------------------------------------- ofxOpenNI
class ofxOpenNIProxy{
public:
	ofxOpenNIProxy();
	
#ifdef HARDWARE
	ofxHardwareDriver	hardware;
#endif
	
	ofxOpenNIContext	context;
	ofxDepthGenerator	depthGenerator;
	ofxUserGenerator	userGenerator;

#ifdef IMAGE
	ofxImageGenerator	imageGenerator;
#endif
	
	float				filterFactor;
	
	void				setup(ofxUser * _users);
	void				setMasking(bool b_);
	
	void				update();
	
private:
	ofxUser	*users;
	bool				isCloud,isMasking;
	void				updateUsers(int _nUsers);
};

#endif
