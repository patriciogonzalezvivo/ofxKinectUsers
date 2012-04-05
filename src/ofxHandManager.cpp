/*
 *  HandManager
 *
 *  Created by Patricio Gonzalez Vivo on 25/02/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

#include "ofxHandManager.h"

ofxHandManager::ofxHandManager(){
	activeHands.clear();
	pasiveHands.clear();
	
	nHands = 0;
	activeId = 0;
	isActive = false;
	
}

ofxHandManager::~ofxHandManager(){
	activeHands.clear();
	pasiveHands.clear();
};

void ofxHandManager::setup(ofxUser * _users, string xmlFile,vector<Perspective> & _perspectives){
	users = _users;
	
	for(int i=0; i < MAX_USERS ;i++){
        users[i].leftId = addHand();
		users[i].left = false;
		users[i].rightId  = addHand();
		users[i].right = false;
    }
	
	string host = "localhost";
	int port = 3333;
	string perspName = "FRONT";
	absolute = true;
	pushClicking = false;
	bool sendZ;
	
	ofxXmlSettings XML;
	if (XML.loadFile(xmlFile)){
		if ( XML.tagExists("TUIO") ){
			isActive = true;
		
			XML.pushTag("TUIO");
			host = XML.getValue("HOST", "localhost");
			port = XML.getValue("PORT", 3333);
			perspName = XML.getValue("INTERFACE","FRONT");
			
			leftTrigger = XML.getValue("TRIGGERS:LEFTHAND", "LEFT");
			rightTrigger = XML.getValue("TRIGGERS:RIGHTHAND", "RIGHT");
			bothTrigger = XML.getValue("TRIGGERS:BOTHHANDS", "BOTH");
		
			absolute = XML.getValue("ABSOLUTE", 0);
		
			if (!absolute){
				handArea.x = XML.getValue("HANDAREA:WIDTH", 0);
				handArea.y = XML.getValue("HANDAREA:HEIGHT", 0);
				handArea.z = XML.getValue("HANDAREA:DEPTH", 0);
			} else {
				handArea.x = 640;
				handArea.y = 480;
				handArea.z = 100;
			}
		
			sendZ = XML.getValue("Z",0);
			pushClicking = XML.getValue("PUSHCLICK", 0);
		
			XML.popTag();
		} 
	} else 
		cout << "XML config file not found" << endl;
	
	tuio.setup(host,port,sendZ,handArea);
	
	for (int i = 0 ; i < _perspectives.size() ; i++){
		if (_perspectives[i].name == perspName){
			perspective = _perspectives[i];
			break;
		}
	}
}

void ofxHandManager::update(){
	for (int i = 0; i < MAX_USERS; ++i)
		if( (users[i].isActive) && (users[i].bones) ){
			update(i);
		}
}

void ofxHandManager::update(int userId){
	if (isActive){
		if( users[userId].pose == leftTrigger )
			activeLeft(userId, users[userId].bonesPoints.left_hand * perspective.mat);
		else 
			deactiveLeft(userId);
	
		if( users[userId].pose == rightTrigger )
			activeRight(userId, users[userId].bonesPoints.right_hand * perspective.mat);
		else 
			deactiveRight(userId);
	
		if( users[userId].pose == bothTrigger ){
			activeLeft(userId, users[userId].bonesPoints.left_hand * perspective.mat);
			activeRight(userId, users[userId].bonesPoints.right_hand * perspective.mat);
		}
	}
}

int ofxHandManager::addHand(){
	Hand h;

	h.id = -1;				// When it becomes active it will have a valid id
	h.centroid.set(0,0,0);
	h.lastCentroid.set(0,0,0);
	h.D.set(0,0,0);
	h.areaCorner.set(0,0,0);

	pasiveHands[nHands] = h;
	nHands++;
	return nHands - 1;
}

void ofxHandManager::updateHand(int _id, ofPoint _pos){
	int now = ofGetElapsedTimeMillis();
	
	activeHands[_id].lastCentroid = activeHands[_id].centroid;
	activeHands[_id].centroid = _pos;
	
	ofPoint tD;
	//get the Differences in position
	//tD.set(activeHands[_id].centroid.x - activeHands[_id].lastCentroid.x, 
	//	   activeHands[_id].centroid.y - activeHands[_id].lastCentroid.y);
	//calculate the acceleration
	//float posDelta = sqrtf((tD.x*tD.x)+(tD.y*tD.y));
	
	//float a = 1.0f - 1.0f / expf(posDelta / (1.0f + (float)MOVEMENT_FILTERING*10));
	//activeHands[_id].centroid.x = a * activeHands[_id].centroid.x + (1-a) * activeHands[_id].lastCentroid.x;
	//activeHands[_id].centroid.y = a * activeHands[_id].centroid.y + (1-a) * activeHands[_id].lastCentroid.y;
	
	//get the Differences in position
	activeHands[_id].D.set(activeHands[_id].centroid.x - activeHands[_id].lastCentroid.x, 
						   activeHands[_id].centroid.y - activeHands[_id].lastCentroid.y,
						   activeHands[_id].centroid.z - activeHands[_id].lastCentroid.z);
	
	tD = activeHands[_id].D;
	activeHands[_id].maccel = sqrtf((tD.x* tD.x)+(tD.y*tD.y)) / (now - activeHands[_id].lastTimeTimeWasChecked);
	
	//calculate the age
	activeHands[_id].age = ofGetElapsedTimef() - activeHands[_id].downTime;
	
	activeHands[_id].lastTimeTimeWasChecked = now;
	
	
	// The blob it will be calculated and store... but not send it unless is
	float zDiff = activeHands[_id].areaCorner.z - _pos.z ;
	if ( (zDiff > pushClicking) )
		activeHands[_id].send = true;
	else
		activeHands[_id].send = false;

}

void ofxHandManager::activeHand(int _handId, ofPoint _startP){ // _startP es el punto inicial desde donde construye el area
	activeHands[_handId] = pasiveHands[_handId];
	
	pasiveHands.erase(_handId);
	
	activeHands[_handId].id = activeId;
	activeId++;
	
	activeHands[_handId].downTime = ofGetElapsedTimef();
	
	// Si es absoluto hace que la esquina del area sea el 0,0,0 de la imagen de la kinect y 
	// al handArea ser seteada en el setup con las dimensiones, hace q pase valores absolutos y normalizados en el ofxTUIO
	// Si no es absoluto los moviemientos de la mano se captar dentro de un rango
	
	if (!absolute)
		activeHands[_handId].areaCorner = ofPoint(_startP.x-handArea.x/2, _startP.y-handArea.x/2, _startP.z-handArea.x/2);
	else
		activeHands[_handId].areaCorner = ofPoint(0,0,0);
}

void ofxHandManager::deactiveHand(int _handId){
	activeHands[_handId].id = -1;
	activeHands[_handId].centroid.set(0,0,0);
	activeHands[_handId].lastCentroid.set(0,0,0);
	activeHands[_handId].D.set(0,0,0);
	activeHands[_handId].maccel = 0;
	activeHands[_handId].areaCorner.set(0,0,0);
	pasiveHands[_handId] = activeHands[_handId];
	
	activeHands.erase(_handId);
}

void ofxHandManager::activeLeft(int _userId, ofPoint _pos){
	for (int i = 0; i < MAX_USERS; i++) {
		if (users[i].userId == _userId){
			if (!(users[i].left)){
				activeHand(users[i].leftId,_pos);
				users[i].left = true;
			} 
			
			updateHand(users[i].leftId, _pos);
		}
	}
};

void ofxHandManager::activeRight(int _userId, ofPoint _pos){
	for (int i = 0; i < MAX_USERS; i++) {
		if (users[i].userId == _userId){
			if (!(users[i].right)){
				activeHand(users[i].rightId,_pos);
				users[i].right = true;
			}
			
			updateHand(users[i].rightId, _pos);
		}
	}
};

void ofxHandManager::deactiveLeft(int _userId){
	for (int i = 0; i < MAX_USERS; i++) {
		if (users[i].userId == _userId){
			if (users[i].left){
				deactiveHand(users[i].leftId);
				users[i].left = false;
			}
		}
	}
};

void ofxHandManager::deactiveRight(int _userId){
	for (int i = 0; i < MAX_USERS; i++) {
		if (users[i].userId == _userId){
			if (users[i].right){
				deactiveHand(users[i].rightId);
				users[i].right = false;
			}
		}
	}
};

void ofxHandManager::drawHandAreas(){
	if ( isActive && !absolute ){
		map<int, Hand>::iterator hand;
		for(hand = activeHands.begin(); hand != activeHands.end(); hand++){
			// omit point (0,0) since this means that we are outside of the range
			if((hand->second.centroid.x <= 0) && (hand->second.centroid.y <= 0) && (hand->second.id == -1))
				continue;
		
			ofSetColor(255);
			ofNoFill();
			ofRect(hand->second.areaCorner.x, hand->second.areaCorner.y, handArea.x, handArea.y);
			ofFill();
		
			//ofCircle(hand->second.centroid.x,hand->second.centroid.y, 0, 20);
			//ofSetColor(255);
			//ofDrawBitmapString(ofToString((int)hand->second.id),hand->second.centroid.x-5,hand->second.centroid.y);
			//ofDrawBitmapString("z:"+ofToString((int)hand->second.centroid.z/100.0f),hand->second.centroid.x-15,hand->second.centroid.y+10);
		}
	}
}

void ofxHandManager::drawOutPut(int x, int y, int w, int h){
	ofNoFill();
	ofRect(x, y, w, h);
	
	map<int, Hand>::iterator hand;
	for(hand = activeHands.begin(); hand != activeHands.end(); hand++){
		// omit point (0,0) since this means that we are outside of the range
		if((hand->second.centroid.x <= 0) && (hand->second.centroid.y <= 0) && (hand->second.id == -1))
			continue;
		
		ofFill();
		if (hand->second.send)
			ofSetColor(0, 255, 0);
		else
			ofSetColor(200);

		ofCircle( x + ((hand->second.centroid.x - hand->second.areaCorner.x) / handArea.x ) * w , 
				  y + ((hand->second.centroid.y - hand->second.areaCorner.y) / handArea.y ) * h , 0, 7);
		
		//ofSetColor(255);
		//ofDrawBitmapString(ofToString((int)hand->second.id),hand->second.centroid.x-5,hand->second.centroid.y);
		//ofDrawBitmapString("z:"+ofToString((int)hand->second.centroid.z/100.0f),hand->second.centroid.x-15,hand->second.centroid.y+10);
	}
}