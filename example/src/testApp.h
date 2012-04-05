#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxKinectUsers.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxKinectUsers	inter;
	void	inPose(ofxUser & user);
	void	inGesture(ofxUser & user);
	void	userIn(ofxUser & user);
	void	userMove(ofxUser & user);
	void	userOut(ofxUser & user);
	
private:
	bool	drawInterfaceDebug;
};
#endif
