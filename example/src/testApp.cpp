#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableAlphaBlending();
	drawInterfaceDebug = true;
	
	inter.setup("config.xml");
	ofAddListener(inter.inPose,this,&testApp::inPose);
	ofAddListener(inter.inGesture,this,&testApp::inGesture);
	ofAddListener(inter.userIn,this,&testApp::userIn);
	ofAddListener(inter.userMove,this,&testApp::userMove);
	ofAddListener(inter.userOut,this,&testApp::userOut);
	
}

//--------------------------------------------------------------
void testApp::update(){
	inter.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 255, 255);
	
	if (drawInterfaceDebug)
		inter.debugDraw();
	}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key){
		case 'i':
			drawInterfaceDebug=!drawInterfaceDebug;
			break;
		case '1':
			inter.setPerspective(0);
			break;
		case '2':
			inter.setPerspective(1);
			break;
		case '3':
			inter.setPerspective(2);
			break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if (drawInterfaceDebug) {
		inter.mouseDragged(x,y,button);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::inPose(ofxUser & user){
	cout << "INPOSE" << endl;
}

void testApp::inGesture(ofxUser & user){
	cout << "INGESTURE" << endl; 
}

void testApp::userIn(ofxUser & user){
	cout << "USERIN" << endl; 
}

void testApp::userMove(ofxUser & user){
	cout << "USERMOVE" << endl; 
}

void testApp::userOut(ofxUser & user){
	cout << "USEROUT" << endl; 
}