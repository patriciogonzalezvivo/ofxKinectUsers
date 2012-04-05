/*
*  TUIO.h
*  
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/
#ifndef TUIO_H
#define TUIO_H

#include "ofxOsc.h"

typedef struct {
	ofPoint centroid, lastCentroid, D;
	ofPoint	areaCorner;
	int		id;
	float	maccel;  //distance traveled since last frame
	float	age;     //how long the blob has been at war
	float	sitting; //how long hes been sitting in the same place
	float	downTime;
	float	lastTimeTimeWasChecked;
	bool	send;
} Hand;

class ofxTUIO {
public:		
	ofxTUIO(){};
	~ofxTUIO(){};
		
	//methods
	void setup(string host, int port, bool _sendZ, ofPoint _handArea) {
		localHost = host;
		TUIOPort = port;
		frameseq = 0;
		handArea = _handArea;
		
		sendZ = _sendZ;
		
		if (sendZ)
			header = "/tuio/3Dcur"; // 3D Structure /tuio/3Dcur set s x y z X Y Z m
		else 
			header = "/tuio/2Dcur"; // 2D Structure /tuio/2Dcur set s x y X Y m
		
		TUIOSocket.setup(localHost.c_str(), TUIOPort);
	};
	
	void send(std::map<int, Hand> * hands){
		frameseq += 1;
		
		ofxOscBundle b;
		ofxOscMessage alive;
		// Sends alive message - saying 'Hey, there's no alive blobs'
		alive.setAddress(header);
		alive.addStringArg("alive");
		
		// Send fseq message
		ofxOscMessage fseq;
		fseq.setAddress( header );
		fseq.addStringArg( "fseq" );
		fseq.addIntArg(frameseq);
		
		if(hands->size() == 0){
			b.addMessage( alive );		// add message to bundle
			b.addMessage( fseq );		// add message to bundle
			TUIOSocket.sendBundle( b ); // send bundle
		} else {
			map<int, Hand>::iterator hand;
			for(hand = hands->begin(); hand != hands->end(); hand++){
				// omit point (0,0) since this means that we are outside of the range
				if((hand->second.centroid.x <= 0) && (hand->second.centroid.y <= 0) && 
				   (hand->second.id == -1) && !hand->second.send )
					continue;
				//Set Message
				ofxOscMessage set;
				set.setAddress( header );
				set.addStringArg("set");
				set.addIntArg(hand->second.id);				// id
				set.addFloatArg( (hand->second.centroid.x - hand->second.areaCorner.x) / handArea.x );		// x
				set.addFloatArg( (hand->second.centroid.y - hand->second.areaCorner.y) / handArea.y );		// y
				
				if (sendZ)		// 3D Structure /tuio/3Dcur set s x y z X Y Z m
					set.addFloatArg( (hand->second.centroid.z - hand->second.areaCorner.z) / handArea.z );	// z
				
				set.addFloatArg( (hand->second.centroid.x - hand->second.areaCorner.x) / handArea.x );		// dX
				set.addFloatArg( (hand->second.centroid.y - hand->second.areaCorner.y) / handArea.y );		// dY
				
				if (sendZ)		// 3D Structure /tuio/3Dcur set s x y z X Y Z m
					set.addFloatArg( (hand->second.centroid.z - hand->second.areaCorner.z) / handArea.z );	// dZ
				
				set.addFloatArg(hand->second.maccel);		// m
				b.addMessage( set );						// add message to bundle
				alive.addIntArg(hand->second.id);			// add blob to list of ALL active IDs
			}
			b.addMessage( alive );	 //add message to bundle
			b.addMessage( fseq );	 //add message to bundle
			TUIOSocket.sendBundle( b ); //send bundle
		}
	};

	ofxOscSender	TUIOSocket; 
	string			localHost;
	int				TUIOPort;
	
	bool			sendZ;
	string			header;

private:
	int				frameseq;
	bool			send(string message);
	ofPoint			handArea;
};
#endif