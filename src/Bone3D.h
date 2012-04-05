/*
 *  Bone3D.h
 *  openNISkeleton
 *
 *  Created by Patricio González Vivo on 04/06/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#ifndef _BONE3D
#define _BONE3D

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxAssimpModelLoader.h"
using namespace xn;

class Bone3D :public ofNode {
public:
	ofQuaternion		 bindPoseOrientation;
	ofxAssimpModelLoader mesh;
	
	int joinId;
	XnSkeletonJoint skelJoint;
	
	Bone3D();
	
	void setJoin(string jointName);
	
	void customDraw();
	
	void customDrawBox();
};

#endif
