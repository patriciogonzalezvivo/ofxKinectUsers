/*
 *  ofxUser.h
 *  ofxKinectUsers
 *
 *  Created by Patricio Gonzalez Vivo on 17/03/11.
 *  Copyright 2011 www.PatricioGonzalezVivo.com All rights reserved.
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to 
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

#ifndef OFXUSER
#define OFXUSER

#include "ofMain.h"
#include "ofxKTools.h"

class ofxUser{
public:
	
	unsigned char *	maskPixels;
	ofVec3f centroid, vel;
	float	velF;
	
	int		userId;
    int		idleTime;
	bool	isIni, isActive, isIdle;
	
	bool	bones;
	//------------------------------ bones
	Bones	bonesPoints, bonesVel;
	BonesF	bonesVelF;
	Area	area;
	Arms	arms;			
	
	string	pose, gesture;
	int		poseTimer, gestureTimer;
	bool	inPose,gestureInit,gestureComp, inGesture;
	
	float	head,palms;
	int		jumpCounter, applauseCounter;
	
	//------------------------------- Hands
	bool	left, right;
	int		leftId, rightId;
	
	ofxUser();
	
	void setup(int _userId, Perspective * _perspective);
    void update();
	
	ofVec3f getPerspectiveCentroid(){return centroid * perspective->mat;};
	ofVec3f getPerspectiveVel(){return vel * perspective->mat;};
	//------------------------------- bones
	Bones	getPerspectiveBones(){return bonesPoints * perspective->mat;};
	Bones	getPerspectiveBonesVel(){return bonesVel * perspective->mat;};
	Area	getPerspectiveArea(){return area * perspective->mat;};

private:
	Perspective *perspective;
	
	ofVec3f lastCentroid;
	Bones	bonesLastPoints;
	
	void reIni();
};
#endif
