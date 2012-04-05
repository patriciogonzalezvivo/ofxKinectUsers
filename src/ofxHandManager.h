/*
 *  ofxHandManager
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

#ifndef HANDMANAGER
#define HANDMANAGER

#include "ofxTUIO.h"
#include "ofxUser.h"
#include "ofxXmlSettings.h"

#define	MAX_USERS 8

class ofxHandManager{
public:
	ofxTUIO			tuio;
	Perspective		perspective;
	
	map<int, Hand>	activeHands;
	map<int, Hand>	pasiveHands;
	int				nHands, activeId, pushClicking;
	bool			isActive, absolute;
	
	ofxHandManager();
	~ofxHandManager();
	
	void			setup(ofxUser * _users, string xmlFile, vector<Perspective> & _perspectives);
	void			update();
	void			update(int userId);
	void			send(){ if(isActive) tuio.send(&activeHands); };
	void			drawHandAreas();
	void			drawOutPut(int x, int y, int w, int h);

private:
	ofxUser		*users;
	
	int			addHand();
	void		updateHand(int _handId, ofPoint _pos);
	
	void		activeHand(int _handId, ofPoint _startPoint);
	void		deactiveHand(int _handId);
	
	void		activeLeft(int _userId, ofPoint _pos);
	void		activeRight(int _userId, ofPoint _pos);
	void		deactiveLeft(int _userId);
	void		deactiveRight(int _userId);
	
	string		leftTrigger,rightTrigger,bothTrigger;
	ofPoint		handArea;
};
#endif