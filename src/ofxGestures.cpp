/*
 *  ofxGestures.cpp
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

#include "ofxGestures.h"

ofxGestures::ofxGestures(){
	isPosesActive = false;
	isGesturesActive = false;
	isNatGesturesActive = false;
}

void ofxGestures::setup(ofxUser *_users, string xmlfile, Perspective * _perspective){
	users = _users;
	perspective = _perspective;
	
	for(int i=0;i<MAX_USERS;i++){
        users[i].arms = setArms(5,5,5,5);
		resetUser(i);
    }
	
	loadPoses(xmlfile); 
	loadGestures(xmlfile);
}

void ofxGestures::resetUser(int userId){
	users[userId].pose	= "";
	users[userId].poseTimer = 0;
	users[userId].inPose = false;
	
	users[userId].gesture	= "";
	users[userId].gestureTimer = 0;
	users[userId].gestureInit = false;
	users[userId].gestureComp = false;
	users[userId].inGesture = false;
	
	users[userId].applauseCounter = 0;
	users[userId].jumpCounter = 0;
	
	//users[userId].altura = 0;
	//users[userId].palmas = 0;
}

bool ofxGestures::loadPoses(string filePath){
	ofxXmlSettings XML;
	cout << "Loading poses file " << filePath;
	
	if (XML.loadFile(filePath)){
		if ( XML.tagExists("POSES") ){
			isPosesActive = true;
			
			cout << " [ OK ]" << endl;
		
			XML.pushTag("POSES");
			for (int i = 0; i < XML.getNumTags("POSE"); i++){
				XML.pushTag("POSE", i);
			
				Pose p;
				p.name		= XML.getValue("NAME","DAVINCI");
				p.poseDetonator = setArms(XML.getValue("LEFT:HAND",4),XML.getValue("LEFT:SHOULDER",6),XML.getValue("RIGHT:SHOULDER",4),XML.getValue("RIGHT:HAND",6));
				p.timerMin		= XML.getValue("TIMEMIN",30);
				poses.push_back(p);
			
				cout << "Pose " << p.name << "( " << p.poseDetonator.left.hand << "," << p.poseDetonator.left.shoulder << "," << p.poseDetonator.right.shoulder << "," << p.poseDetonator.right.hand << " )" <<" [ LOADED ] "<< endl;
			
				XML.popTag();
			}
			XML.popTag();
		}
	} else
		cout << " [ FAIL ]" << endl;
	
	return isPosesActive;
}

bool ofxGestures::loadGestures(string filePath){
	ofxXmlSettings XML;
	cout << "Loading gesures file " << filePath;
	bool load = false;
	
	if (XML.loadFile(filePath)){
		if ( XML.tagExists("GESTURES") ){
			isGesturesActive = true;
			cout << " [ OK ]" << endl;
		
			XML.pushTag("GESTURES");
			for (int i = 0; i < XML.getNumTags("GESTURE"); i++){
				XML.pushTag("GESTURE", i);
			
				Gesture g;
				g.name		= XML.getValue("NAME","FLY");
				g.timerMax		= XML.getValue("TIMEMAX",30);
				g.initPose = setArms(XML.getValue("BEGIN:LEFT:HAND",4),XML.getValue("BEGIN:LEFT:SHOULDER",3),XML.getValue("BEGIN:RIGHT:SHOULDER",1),XML.getValue("BEGIN:RIGHT:HANDO",6));
				g.finalPose = setArms(XML.getValue("END:LEFT:HAND",4),XML.getValue("END:LEFT:SHOULDER",3),XML.getValue("END:RIGHT:SHOULDER",1),XML.getValue("END:RIGHT:HANDO",6));			
				gestures.push_back(g);
			
				cout << "Gesture " << g.name << " [ LOADED ] "<< endl;
			
				XML.popTag();
			}
			XML.popTag();
		}
		if ( XML.tagExists("NATGESTURES") ){
			isNatGesturesActive = true;
			XML.pushTag("NATGESTURES");
			aDistMin	= XML.getValue("APPLAUSE:DISTMIN", 20);
			aDistMax	= XML.getValue("APPLAUSE:DISTMAX", 50);
			aTimeMax	= XML.getValue("APPLAUSE:TIMEMAX", 30);
			aTimes		= XML.getValue("APPLAUSE:TIMES", 2);
		
			jAltMin		= XML.getValue("JUMP:ALTMIN", -10);
			jAltMax		= XML.getValue("JUMP:ALTMAX", 10);
			jTimeMax	= XML.getValue("JUMP:TIMEMAX", 30);
			jTimes		= XML.getValue("JUMP:TIMES", 2);
			XML.popTag();
		}
		
	} else
		cout << " [ FAIL ]" << endl;
	
	return (isGesturesActive || isNatGesturesActive);
}

//--------------------------------------------------------------
void ofxGestures::update(){
	for (int i = 0; i < MAX_USERS; ++i)
		if( (users[i].isActive) && (users[i].bones) )
			update(i);
}

void ofxGestures::update(int userId){
	if ( isPosesActive || isGesturesActive || isNatGesturesActive )
		users[userId].arms = getArms(userId);
			
	if (isPosesActive) checkPoses(userId);
	if (isGesturesActive) checkGestures(userId);
	if (isNatGesturesActive) checkNatGestures(userId);
}

Arms ofxGestures::setArms(int leftHand, int leftShoulder, int rightShoulder, int rightHand){
	Arms a;
	
	a.right.shoulder = rightShoulder;
	a.right.hand = rightHand;
	
	a.left.shoulder = leftShoulder;
	a.left.hand	= leftHand;

	return a;
}

Arms ofxGestures::getArms(int userId){
	Arms a;
	
	a.right = getArm(userId,true);
	a.left = getArm(userId,false);
	
	return a;
}

Arm ofxGestures::getArm(int userId, bool right){
	float goldNumber = users[userId].bonesPoints.torso.y - users[userId].bonesPoints.head.y;
	float tolerance = (10*goldNumber)/100;
	//float tolerance = 1;
	
	Arm a;
	
	if (right){
		a.shoulder = toMat(users[userId].bonesPoints.right_shoulder * perspective->mat,users[userId].bonesPoints.right_elbow * perspective->mat, tolerance);
		a.hand = toMat(users[userId].bonesPoints.right_hand * perspective->mat,users[userId].bonesPoints.right_elbow * perspective->mat, tolerance);
	} else {
		a.shoulder = toMat(users[userId].bonesPoints.left_shoulder * perspective->mat, users[userId].bonesPoints.left_elbow * perspective->mat, tolerance);
		a.hand = toMat(users[userId].bonesPoints.left_hand * perspective->mat, users[userId].bonesPoints.left_elbow * perspective->mat, tolerance);
	}
	
	return a;
}

int	ofxGestures::toMat(ofVec3f shoulderOrHand, ofVec3f elbow, float tolerance){
	int mat[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	int x = 1;
	int y = 1;
	
	float difX = shoulderOrHand.x - elbow.x;
	float difY = shoulderOrHand.y - elbow.y;
	
	if (difX < 0 - tolerance)
		x = 0;
	else if (difX > 0 + tolerance)
		x = 2;
	
	if (difY < 0 - tolerance*2)
		y = 0;
	else if (difY > 0 + tolerance*2)
		y = 2;
	
	return mat[y][x];
	
	//
	//	[1]		[2]		[3]
	//	-/-		0/-		+/-
	// 
	//	[4]		[5]		[6]
	//	-/0		0/0		+/0
	//
	//	[7]		[8]		[9]
	//	-/+		0/+		+/+
	//
}

bool ofxGestures::checkPoses(int userId){
	bool act = false;
	
	for (int j = 0; j < poses.size(); j++){
		if (poses[j].poseDetonator == users[userId].arms){
			act = true;
			
			if (poses[j].name != users[userId].pose){
				users[userId].pose = poses[j].name;
				users[userId].poseTimer = 1;
				users[userId].inPose = false;
			} else	
				users[userId].poseTimer++;
			
			if ( users[userId].poseTimer >= poses[j].timerMin){
				resetUser(userId);
				users[userId].pose = poses[j].name;
				users[userId].inPose = true;
			}
		}
	}
	
	if (!act) {
		users[userId].pose	=	"";
		users[userId].inPose = false;
	}
	
	return act;
}

bool ofxGestures::checkGestures(int userId){
	bool act = false;
	
	if ( !(users[userId].gestureInit) ){
		for (int j = 0; j < gestures.size(); j++){
			
			if (gestures[j].initPose == users[userId].arms){
				act = true;
				
				users[userId].gesture = gestures[j].name;
				users[userId].gestureTimer = 1;
				users[userId].gestureInit = true;
				users[userId].gestureComp = false;	
				users[userId].inGesture = false;			
				
				users[userId].pose = "";
				users[userId].inPose = false;
			}
		}
	} else if ( !(users[userId].gestureComp) ){
		for (int j = 0; j < gestures.size(); j++){
			
			if ( (gestures[j].name == users[userId].gesture)){
				
				if (users[userId].gestureTimer >= gestures[j].timerMax){
					resetUser(userId);
				} else if (users[userId].arms == gestures[j].finalPose){	
					act = true;
					users[userId].gestureComp = true;
				}else 
					users[userId].gestureTimer++;
			}
		}
	} else {
		for (int j = 0; j < gestures.size(); j++){
			
			if ( (gestures[j].name == users[userId].gesture)) {
				
				if (users[userId].gestureTimer >= gestures[j].timerMax){
					resetUser(userId);
				} else if (users[userId].arms == gestures[j].initPose){
					act = true;
					users[userId].inGesture = true;
					users[userId].gestureTimer = 1;
				} else 
					users[userId].gestureTimer++;
			}
		}
	}
	
	return act;
}

bool ofxGestures::checkNatGestures(int userId){
	bool act = false;
	float head = users[userId].bonesPoints.head.y;
	float palms = ofDist(users[userId].bonesPoints.left_hand.x,users[userId].bonesPoints.left_hand.y,users[userId].bonesPoints.right_hand.x,users[userId].bonesPoints.right_hand.y);
	
	if ( !(users[userId].gestureInit) ){
		
		if (palms < aDistMin){
			act = true;
			users[userId].gesture = "APPLAUSE";
			users[userId].gestureTimer = 1;
			users[userId].gestureInit = true;
			users[userId].gestureComp = false;
			users[userId].inGesture = false;			
			
			users[userId].pose = "";
			users[userId].inPose = false;
			
		} else if (users[userId].head - head > jAltMax){
			act = true;
			users[userId].gesture = "JUMP";
			users[userId].gestureTimer = 1;
			users[userId].gestureInit = true;
			users[userId].gestureComp = false;	
			users[userId].inGesture = false;			
			
			users[userId].pose = "";
			users[userId].inPose = false;											
		}
	} else if ( !(users[userId].gestureComp) ){
		
		if ( users[userId].gesture == "APPLAUSE"){ 
			if (users[userId].gestureTimer >= aTimeMax){
				resetUser(userId);
			} else if (palms > aDistMax){
				act = true;
				users[userId].gestureComp = true;
			}else 
				users[userId].gestureTimer++;
		} else if ( users[userId].gesture == "JUMP" ){
			if (users[userId].gestureTimer >= jTimeMax){
				resetUser(userId);
			} else if (users[userId].head - head < jAltMin){
				act = true;
				users[userId].gestureComp = true;
			}else 
				users[userId].gestureTimer++;
		} 
		
	} else {
		if ( users[userId].gesture == "APPLAUSE"){
			
			if (users[userId].gestureTimer >= aTimeMax){
				resetUser(userId);
			} else if (palms < aDistMin){
				users[userId].gestureTimer = 1;
				act = true;
				
				users[userId].applauseCounter++;
				
				if (users[userId].applauseCounter >= aTimes)
					users[userId].inGesture = true;				
				
			}else 
				users[userId].gestureTimer++;
			
		} else if ( users[userId].gesture == "JUMP" ){
			
			if (users[userId].gestureTimer >= jTimeMax){
				resetUser(userId);
			} else if (users[userId].head - head > jAltMax){
				users[userId].gestureTimer = 1;
				act = true;
				
				users[userId].jumpCounter++;
				if (users[userId].jumpCounter >= jTimes)
					users[userId].inGesture = true;
				
			}else 
				users[userId].gestureTimer++;
		} 
	}
	
	users[userId].head = head;
	users[userId].palms = palms;
	
	return act;
}


//--------------------------------------------------------------
void ofxGestures::debugDraw(){

	//			,           ________             ,    
	//        =4>-----5-----6      |------o------<0=
	//         int         int    int            int
	//
	if ( isPosesActive || isGesturesActive || isNatGesturesActive )
	for (int i = 0; i < MAX_USERS; ++i){
		if((users[i].isActive) && (users[i].bones)){
			float goldNumber = users[i].bonesPoints.torso.y - users[i].bonesPoints.head.y;
			float tolerance = (10*goldNumber)/100;
			float radio = 10; 
			
			ofSetColor(0,255,0);
			ofCircle(users[i].bonesPoints.right_shoulder.x, users[i].bonesPoints.right_shoulder.y, radio);
			ofSetColor(0);
			ofDrawBitmapString(ofToString(toMat(users[i].bonesPoints.right_shoulder * perspective->mat,users[i].bonesPoints.right_elbow * perspective->mat, tolerance)),
							   users[i].bonesPoints.right_shoulder.x-radio/2,users[i].bonesPoints.right_shoulder.y+radio/2);
			
			ofSetColor(0,255,0);
			ofCircle(users[i].bonesPoints.right_hand.x,users[i].bonesPoints.right_hand.y, radio);
			ofSetColor(0);
			ofDrawBitmapString(ofToString(toMat(users[i].bonesPoints.right_hand * perspective->mat,users[i].bonesPoints.right_elbow * perspective->mat, tolerance)),
							   users[i].bonesPoints.right_hand.x-radio/2,users[i].bonesPoints.right_hand.y+radio/2);
			
			ofSetColor(0,255,0);
			ofCircle(users[i].bonesPoints.left_shoulder.x,users[i].bonesPoints.left_shoulder.y, radio);
			ofSetColor(0);
			ofDrawBitmapString(ofToString(toMat(users[i].bonesPoints.left_shoulder * perspective->mat,users[i].bonesPoints.left_elbow * perspective->mat, tolerance)),
							   users[i].bonesPoints.left_shoulder.x-radio/2,users[i].bonesPoints.left_shoulder.y+radio/2);
			
			ofSetColor(0,255,0);
			ofCircle(users[i].bonesPoints.left_hand.x,users[i].bonesPoints.left_hand.y, radio);
			ofSetColor(0);
			ofDrawBitmapString(ofToString(toMat(users[i].bonesPoints.left_hand * perspective->mat,users[i].bonesPoints.left_elbow * perspective->mat, tolerance)),
							   users[i].bonesPoints.left_hand.x-radio/2,users[i].bonesPoints.left_hand.y+radio/2);
			
			ofPushMatrix();
			
			ofTranslate(users[i].centroid.x,users[i].centroid.y,0);
			if (users[i].inPose)
				ofSetColor(0,255,0);
			else
				ofSetColor(100);
			ofDrawBitmapString("Pose: "+  users[i].pose, -40, -5);
			if (users[i].inGesture)
				ofSetColor(0,255,0);
			else
				ofSetColor(100);
			ofDrawBitmapString("Gest: "+  users[i].gesture, -40, 5);
			ofPopMatrix();
		}
	}
}