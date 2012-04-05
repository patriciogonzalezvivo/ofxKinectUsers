/*
 *  ofxKinectUsers.cpp
 *  ofxKinectUsers
 *
 *  Created by Patricio Gonzalez Vivo on 17/03/11.
 *  Copyright 2011 www.PatricioGonzalezVivo.com All rights reserved.
 *
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to 
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */
#define	MAX_USERS 8
//#define KINECT_HARDWARE

#ifndef OFXKINECTUSERS
#define OFXKINECTUSERS

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "ofxOpenNIProxy.h"
#include "ofxKTools.h"
#include "ofxGestures.h"
#include "ofxUser.h"
#include "ofxHandManager.h"

class ofxKinectUsers{
public:
	ofxUser	users[MAX_USERS];
	int		nUsers, nBones;
	
	Area	dArea;
	
	ofxKinectUsers();
	
	void	setup(string _configFile);
	
	void	setMasking(bool _b){ openni.setMasking(_b); };
	void	setPerspective(int _number);
	void	setPerspective(string _name);
	
	ofxTrackedUser* getTrackedUser(int _nUser){
		return openni.userGenerator.getTrackedUser(_nUser);
	}
	
	void	update();
	void	debugDraw();
	
	void	mousePressed(int x, int y, int button);
	void	mouseDragged(int x, int y, int button);
	
	ofEvent<ofxUser> inPose;
	ofEvent<ofxUser> inGesture;
	ofEvent<ofxUser> userIn;
	ofEvent<ofxUser> userMove;
	ofEvent<ofxUser> userOut;

private:
	ofxOpenNIProxy	openni;
	ofxGestures		gestures;
	ofxHandManager	hands;
	ofImage			logo;
	string			configFile;
	
	Perspective		defaultPerspective;
	void			loadPerspectives(string xmlFile);
	void			loadDetectionArea(string xmlFile);
	void			saveDetectionArea(string xmlFile);
	vector<Perspective> perspectives;
	int nPerspective;
};
#endif
