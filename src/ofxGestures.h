/*
 *  ofxGestures.h
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

#ifndef OFXGESTURES
#define OFXGESTURES

#define	MAX_USERS 8

#include "ofMain.h"
#include <XnOpenNI.h>
#include "ofxOpenNIProxy.h"
#include "ofxXmlSettings.h"

#include "ofxUser.h"

//---------------------------------- GESTURES MANAGER
class ofxGestures{
public:
	ofxGestures();
	
	void	setup(ofxUser * _users, string xmlFile, Perspective * _perspective);
	void	update();
	void	update(int userId);

	void	debugDraw();
	
private:
	ofxUser	*users;
	Perspective *perspective;
	
	vector<Pose>	poses;
	vector<Gesture>	gestures;
	
	float	aDistMin,aDistMax;
	int		aTimeMax;
	int		aTimes;
	float	jAltMin,jAltMax;
	int		jTimeMax;
	int		jTimes;
	
	bool	loadPoses(string path);
	bool	loadGestures(string path);
	
	bool	checkPoses(int userId);
	bool	checkGestures(int userId);
	bool	checkNatGestures(int userId);
	
	void	resetUser(int userId);
	
	Arms	setArms(int leftHand, int leftShoulder, int rightShoulder, int rightHand); 
	Arms	getArms(int userId);	
	Arm		getArm(int userId, bool right);	
	int		toMat(ofVec3f shoulderOrHand, ofVec3f elbow, float tolerance);
	
	//
	//						  (  )
	//			,           ___||___             ,    
	//        =0>-----o-----|      |------o------<0=
	//
	//		   1		   2	  3				  4
	//         int         int    int            int
	//
	
	//
	//					[1]		[2]		[3]
	//					-/-		0/-		+/-
	// 
	//					[4]		[5]		[6]
	//					-/0		0/0		+/0
	//
	//					[7]		[8]		[9]
	//					-/+		0/+		+/+
	//
	
	bool	isPosesActive,isGesturesActive,isNatGesturesActive;
};
#endif



