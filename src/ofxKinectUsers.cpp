/*
 *  ofxKinectUsers.h
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

#include "ofxKinectUsers.h"

/////////////////////////////////////////////////////////////////// SETUP
//--------------------------------------------------------------------------------------------

// Some default initial configuration flags
ofxKinectUsers::ofxKinectUsers(){
	nPerspective = -1;
	setMasking(false);
}

// Setup and initiation of managers and configuration list
void ofxKinectUsers::setup(string _configFile){
	logo.loadImage("logo.png");
    configFile = _configFile;					// xml file use to config everything

	openni.setup(users);						// openNI Init
	
	loadPerspectives(configFile);				// Loading Perspectives
	
	for(int i = 0 ; i < MAX_USERS; i++)
		users[i].setup(i, &defaultPerspective);	// All users resive an id and default perspective
	
	loadDetectionArea(configFile);				// Loading Detection Area
	
	gestures.setup(users,configFile, 
				   &defaultPerspective);		// Loading Gestures
	
	hands.setup(users,configFile,perspectives);	// Loading Hand Manager for sending TUIO positions of the hands
}

void ofxKinectUsers::loadPerspectives(string filePath){
	ofxXmlSettings XML;
	cout << "Loading perspectives: " << filePath;
	
	if (XML.loadFile(filePath)){
		cout << " [ OK ]" << endl;
		
		XML.pushTag("PERSPECTIVES");
		for (int i = 0; i < XML.getNumTags("PERSPECTIVE"); i++){
			XML.pushTag("PERSPECTIVE", i);
			
			Perspective p;
			p.name = XML.getValue("NAME","LATERAL");
			ofMatrix4x4 m;
			p.mat = m;
			
			cout << " Loading perspective " << p.name << " y ";
			for (int j = 0; j < XML.getNumTags("SCALE"); j++){
				XML.pushTag("SCALE", j);
				p.mat.scale(XML.getValue("X",1.0),XML.getValue("Y",1.0),XML.getValue("Z",1.0));
				XML.popTag();
				cout << " SCALE ";
			}
			
			for (int j = 0; j < XML.getNumTags("ROTATE"); j++){
				XML.pushTag("ROTATE", j);
				p.mat.rotate(XML.getValue("ANGLE",90.0),XML.getValue("X",0.0),XML.getValue("Y",0.0),XML.getValue("Z",0.0));
				XML.popTag();
				cout << " ROTATE ";
			}
			
			for (int j = 0; j < XML.getNumTags("TRANSLATE"); j++){
				XML.pushTag("TRANSLATE", j);
				p.mat.translate(XML.getValue("X",0.0),XML.getValue("Y",0.0),XML.getValue("Z",0.0));
				XML.popTag();
				cout << " TRANSLATE ";
			}
			
			perspectives.push_back(p);
			cout << " [OK] " << endl;
			XML.popTag();
		}
		
		if (XML.tagExists("DEFAULT")){
			setPerspective(XML.getValue("DEFAULT", "FRONT"));
		}
		
		XML.popTag();
	} else
		cout << " [ FAIL ]" << endl;
}

void ofxKinectUsers::loadDetectionArea(string filePath){
	ofxXmlSettings XML;
	cout << "Loading detection area set up: " << filePath;
	
	if (XML.loadFile(filePath)){
		cout << " [ OK ]" << endl;
		
		XML.pushTag("AREA");
		
		XML.pushTag("MIN");
		ofVec3f minV;
		minV.set(XML.getValue("X",0.0),XML.getValue("Y",0.0),XML.getValue("Z",0.0));
		XML.popTag();
		
		XML.pushTag("MAX");
		ofVec3f maxV;
		maxV.set(XML.getValue("X",640.0),XML.getValue("Y",480.0),XML.getValue("Z",6000.0));
		XML.popTag();
		
		dArea.set(minV,maxV);
		
		XML.popTag();
	} else
		cout << " [ FAIL ]" << endl;
}

void ofxKinectUsers::saveDetectionArea(string filePath){
	ofxXmlSettings XML;
	cout << "Saving detection area: " << filePath;
	
	if (XML.loadFile(filePath)){
		XML.pushTag("AREA");
		
		XML.setValue("MIN:X", dArea.min.x);
		XML.setValue("MIN:Y", dArea.min.y);
		XML.setValue("MIN:Z", dArea.min.z);
		
		XML.setValue("MAX:X", dArea.max.x);
		XML.setValue("MAX:Y", dArea.max.y);
		XML.setValue("MAX:Z", dArea.max.z);
		
		XML.popTag();
		XML.saveFile(filePath);
		cout << " [ SAVED ]" << endl;
	} else
		cout << " [ FAIL ]" << endl;
}

void ofxKinectUsers::setPerspective(int _n){
	if(( _n < perspectives.size() ) && (_n > -1) ) { 
		defaultPerspective = perspectives[_n]; 
		nPerspective = _n;
	}
}

void ofxKinectUsers::setPerspective(string _name){
	for (int i = 0 ; i < perspectives.size() ; i++){
		if (perspectives[i].name == _name){
			defaultPerspective = perspectives[i];
			nPerspective = i;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////// UPDATE
//--------------------------------------------------------------------------------------------

void ofxKinectUsers::update(){
	openni.update();
	
	nUsers = nBones = 0;
	float verticalNorm = 0;
	
	for(int i = 0 ; i < MAX_USERS; i++){
		users[i].update();
		
		if ( users[i].isIni ){
			if ( dArea.check3D( users[i].centroid ) ){
				if ( !users[i].isActive ){
					users[i].isActive = true;
					users[i].isIdle	= false;
					users[i].idleTime = 0;
					ofNotifyEvent(userIn, users[i],this);
				} else
					if ((users[i].velF != 0) )//|| (users[i].bonesVelF != 0))
						ofNotifyEvent(userMove, users[i],this);
				
				if ( !users[i].isIdle ){
					nUsers++;
					verticalNorm += users[i].centroid.y;
					if ( users[i].bones ){ 
						nBones++;
						
						gestures.update(i);
						hands.update(i);
						
						if (users[i].inPose)
							ofNotifyEvent(inPose, users[i],this);
						
						if (users[i].inGesture)
							ofNotifyEvent(inGesture, users[i],this);
					}
				}
			} else {						
				if (users[i].isActive){	
					users[i].isActive = false;
					users[i].isIdle	= true;
					users[i].bones = false;
					ofNotifyEvent(userOut, users[i],this);
				}
			}
		}
    }
	hands.send();
}

///////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

void ofxKinectUsers::mouseDragged(int _x, int _y, int _button){
	if (_button == 0){
		if (ofDist(_x, _y, dArea.min.x, dArea.min.y) <= 30){
			dArea.min.x = _x;
			dArea.min.y = _y;
			dArea.update();
			saveDetectionArea(configFile);
		} else if (ofDist(_x, _y, dArea.max.x, dArea.max.y) <= 30){
			dArea.max.x = _x;
			dArea.max.y = _y;
			dArea.update();
			saveDetectionArea(configFile);
		} else if ( _x > 970-300 && _y > 550-30 )
			ofLaunchBrowser("http://www.patriciogonzalezvivo.com");
		//else 
			//ofLaunchBrowser("http://www.patriciogonzalezvivo.com");
	} else if (_button == 2){
		if ( (_x >= dArea.min.x-20) && (_x <= dArea.min.x+20) ){
			dArea.min.z = ofMap(_y, dArea.min.y, dArea.max.y, 0, 6000, true);
			dArea.update();
			saveDetectionArea(configFile);
		} else if ( (_x >= dArea.max.x-20) && (_x <= dArea.max.x+20) ){
			dArea.max.z = ofMap(_y, dArea.min.y, dArea.max.y, 0, 6000, true);;
			dArea.update();
			saveDetectionArea(configFile);
		} 
	} 
}

void ofxKinectUsers::mousePressed(int x, int y, int button) {
	mouseDragged(x, y, button);
}

/////////////////////////////////////////////////////////////////// DRAW
//--------------------------------------------------------------------------------------------
void ofxKinectUsers::debugDraw(){
	ofSetColor(0,255);
	ofRect(0, 0, 970,550);
	ofSetColor(255, 200);
	openni.depthGenerator.draw(0,0,640,480);
	
	// -------------------- Border
	ofNoFill();
	ofSetLineWidth(2);
	ofRect(0, 0, 640, 480);
	// -------------------- Detection Area
	ofSetColor(255,100);
	dArea.draw();
	// -------------------- Area Buttons
	ofRect(dArea.min.x-20, dArea.min.y-20, 40, 40);
	ofRect(dArea.max.x-20, dArea.max.y-20, 40, 40);
	// -------------------- Data: Fps, Users, Bones, accelerometers & Tilt Angle
	ofSetColor(200,255);
	ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 5, 495);
	ofDrawBitmapString("Users: " + ofToString(nUsers), 105, 495);
	ofDrawBitmapString("Bones: " + ofToString(nBones), 205, 495);
	if (hands.isActive)
		ofDrawBitmapString("Sending TUIO cursors to " + hands.tuio.localHost + " : " + ofToString(hands.tuio.TUIOPort), 305, 495);
#ifdef KINECT_HARDWARE
	ofPoint acc = openni.hardware.getAccelerometers();
	ofDrawBitmapString("X: " + ofToString(acc.x,3),  5, 525);
	ofDrawBitmapString("Y: " + ofToString(acc.y,3), 105,525);
	ofDrawBitmapString("Z: " + ofToString(acc.z,3), 205,525);
	ofDrawBitmapString("Angle: " + ofToString(openni.hardware.getTiltAngle(),3), 315, 525);
#endif
	
	// -------------------- Perspectives
	float pHeight[perspectives.size()];	// Set the Y position for arrange the perpective one on the buttom of the other
	for (int j = 0; j < perspectives.size(); j++){
		Area area;		// Total Perspective Area look at that perspective
		area.set(ofVec3f(0,0,0),ofVec3f(640,480,6000));
		area.transform(perspectives[j].mat);
		
		Area pDArea;	// Selected Area in that perspective
		pDArea = dArea;
		pDArea.transform(perspectives[j].mat);
		
		if (j + 1 <= perspectives.size()) {
			float h = area.max.y + 15;
			pHeight[j+1] = pHeight[j] + h*0.2 + 15;
		}
		
		ofPushMatrix();
		
		ofTranslate(640+15,pHeight[j], 0);
		
		if (nPerspective == j){
			ofSetColor(255,255);
			ofDrawBitmapString(" -> Gestures & Poses Manager", 50, 10);
		} else 
			ofSetColor(200,255);
		
		ofDrawBitmapString(perspectives[j].name, 5, 10);
		ofTranslate(0, 15, 0);
		ofScale(0.1,0.1,0.1);
		area.draw();
		
		ofSetColor(255,100);
		pDArea.draw();
		
		ofPopMatrix();
	}
	//-------------------- TUIO output window
	if (hands.isActive){
		ofSetColor(200,255);
		ofDrawBitmapString("TUIO cursors from " + hands.perspective.name + " view", 
						   640 + 15, pHeight[perspectives.size()-1] + 50 - 5);
		hands.drawOutPut(640 + 15, pHeight[perspectives.size()-1] + 50, 
						 300 , 480 - pHeight[perspectives.size()-1] - 50);
	}
	ofFill();
	
	// ---------------------------------------- BLOBS y ESKELETOS
	for(int i = 0 ; i < MAX_USERS; i++){
		if (users[i].isActive){
			ofSetLineWidth(2);
			ofSetColor(255, 0, 0);
			ofPushMatrix();
			ofTranslate(users[i].centroid.x,users[i].centroid.y);
			ofPoint end = users[i].vel * users[i].velF;
			ofLine(0, 0, end.x, end.y);
			ofPopMatrix();
			
			if (users[i].bones){
				// Draw Bones
				ofSetLineWidth(1);
				ofSetColor(0, 255, 0, 255);
				users[i].bonesPoints.draw();
				ofSetColor(0, 255, 100, 100);
				users[i].area.draw();
				
				// Draw Perspectives
				for (int j = 0; j < perspectives.size(); j++){
					Bones temp;
					temp = users[i].bonesPoints;
					temp.transform(perspectives[j].mat);
					
					Area area;
					area = users[i].area;
					area.transform(perspectives[j].mat);
					
					ofPushMatrix();
					ofTranslate(640+15, pHeight[j] + 15 , 0);
					ofScale(0.1,0.1,0.1);
					
					if (nPerspective == j)
						ofSetColor(0,255, 0, 255);
					else 
						ofSetColor(0, 200, 100, 200);
					temp.draw();
					
					if (nPerspective == j)
						ofSetColor(0,255, 0, 100);
					else 
						ofSetColor(0, 200, 100, 100);
					area.draw();
					ofPopMatrix();
				}
				
				// Draw Velocity Vectors
				ofSetLineWidth(2);
				ofSetColor(255, 0, 0);
				for (int j = 0; j < 15; j++){
					ofPushMatrix();
					ofTranslate(users[i].bonesPoints.bone[j]->x,users[i].bonesPoints.bone[j]->y);
					ofLine(0, 0, users[i].bonesVel.bone[j]->x, users[i].bonesVel.bone[j]->y);
					ofPopMatrix();
				}
			} else {
				ofSetLineWidth(1);
					
				for (int j = 0; j < perspectives.size(); j++){
					ofVec3f pos = users[i].centroid;
					pos = pos * perspectives[j].mat;
						
					// Draw Users with no Bones
					ofPushMatrix();
					ofTranslate(640+15, pHeight[j] + 15 , 0);
					ofScale(0.1,0.1,0.1);
					if (nPerspective == j)
						ofSetColor(0,255, 0, 150);
					else 
						ofSetColor(0, 200, 100, 150);
					ofCircle(pos.x-50,pos.y-50, 50);
					ofPopMatrix();
				}
			}
		}
	}
	
	// Draw Gestures
	gestures.debugDraw();
	
	// Draw the Areas for the hand traking Hands 
	hands.drawHandAreas();
	
	ofFill();
	ofSetColor(250, 255);
	ofDrawBitmapString("Patricio Gonzalez Vivo.com",700, 540);
	logo.draw(910, 495, 47,50.6);
}