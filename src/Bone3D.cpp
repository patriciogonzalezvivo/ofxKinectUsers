/*
 *  Bone3D.cpp
 *  openNISkeleton
 *
 *  Created by Patricio González Vivo on 04/06/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 */

#include "Bone3D.h"

Bone3D::Bone3D(){
	
}

void Bone3D::setJoin(string joinName){
	if (joinName == "HEAD"){
		joinId = 1; 
		skelJoint = XN_SKEL_HEAD; 
	} else if ( joinName == "NECK"){
		joinId = 2; 
		skelJoint = XN_SKEL_NECK;
	} else if ( joinName == "TORSO"){
		joinId = 3; 
		skelJoint = XN_SKEL_TORSO; 
	} else if ( joinName == "WAIST"){
		joinId = 4; 
		skelJoint = XN_SKEL_WAIST; 
	} else if ( joinName == "LEFT_COLLAR"){
		joinId = 5; 
		skelJoint = XN_SKEL_LEFT_COLLAR;
	} else if ( joinName == "LEFT_SHOULDER"){
		joinId = 6; 
		skelJoint = XN_SKEL_LEFT_SHOULDER; 
	} else if ( joinName == "LEFT_ELBOW"){
		joinId = 7; 
		skelJoint = XN_SKEL_LEFT_ELBOW; 
	} else if ( joinName == "LEFT_WRIST"){
		joinId = 8; 
		skelJoint = XN_SKEL_LEFT_WRIST; 
	} else if ( joinName == "LEFT_HAND"){
		joinId = 9; 
		skelJoint = XN_SKEL_LEFT_HAND; 
	} else if ( joinName == "LEFT_FINGERTIP"){
		joinId = 10; 
		skelJoint = XN_SKEL_LEFT_FINGERTIP; 
	} else if ( joinName == "RIGHT_COLLAR"){
		joinId	= 11; 
		skelJoint = XN_SKEL_RIGHT_COLLAR;
	} else if ( joinName == "RIGHT_SHOULDER"){
		joinId = 12; 
		skelJoint = XN_SKEL_RIGHT_SHOULDER; 
	} else if ( joinName == "RIGHT_ELBOW"){
		joinId = 13; 
		skelJoint = XN_SKEL_RIGHT_ELBOW; 
	} else if ( joinName == "RIGHT_WRIST"){
		joinId = 14; 
		skelJoint = XN_SKEL_RIGHT_WRIST; 
	} else if ( joinName == "RIGHT_HAND"){
		joinId = 15; 
		skelJoint = XN_SKEL_RIGHT_HAND; 
	} else if ( joinName == "RIGHT_FINGERTIP"){
		joinId = 16; 
		skelJoint = XN_SKEL_RIGHT_FINGERTIP; 
	} else if ( joinName == "LEFT_HIP"){
		joinId = 17; 
		skelJoint = XN_SKEL_LEFT_HIP; 
	} else if ( joinName == "LEFT_KNEE"){
		joinId = 18; 
		skelJoint = XN_SKEL_LEFT_KNEE; 
	} else if ( joinName == "LEFT_ANKLE"){
		joinId = 19; 
		skelJoint = XN_SKEL_LEFT_ANKLE; 
	} else if ( joinName == "LEFT_FOOT"){
		joinId = 20; 
		skelJoint = XN_SKEL_LEFT_FOOT; 
	} else if ( joinName == "RIGHT_HIP"){
		joinId = 21; 
		skelJoint = XN_SKEL_RIGHT_HIP;
	} else if ( joinName ==  "RIGHT_KNEE"){
		joinId = 22; 
		skelJoint = XN_SKEL_RIGHT_KNEE;
	} else if ( joinName == "RIGHT_ANKLE"){
		joinId = 23; 
		skelJoint = XN_SKEL_RIGHT_ANKLE; 
	} else if ( joinName == "RIGHT_FOOT"){
		joinId = 24; 
		skelJoint = XN_SKEL_RIGHT_FOOT;		
	}
}

void Bone3D::customDraw(){
	ofSetColor(255, 255, 255);
	glPushMatrix();
	glColor3ub(255, 255, 255);
	
	mesh.drawFaces();
	
	glPopMatrix();
}


void Bone3D::customDrawBox() {
	ofSetColor(255, 255, 255);
	glPushMatrix();
	glTranslatef(0, -40/2, 0);
	glScalef(3, 30, 3);
	glColor3ub(255, 255, 255);
	ofBox(0, 0, 0, 1);
	glPopMatrix();		
}