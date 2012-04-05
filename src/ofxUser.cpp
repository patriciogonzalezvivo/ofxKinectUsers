/*
 *  ofxUser.cpp
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

#include "ofxUser.h"

ofxUser::ofxUser(){
	userId	= 0;
	isIni	= false;
	isIdle  = true;
	isActive = false;
	bones	= false;
}

void ofxUser::reIni(){
	isIni	= true;
	isIdle	= true;
	idleTime = 0;
	
	bones = false;
	bonesPoints.set(0, 0, 0);
	bonesVel.set(0, 0, 0);
	bonesLastPoints.set(0, 0, 0);
	
	centroid.set(0,0,0);
	lastCentroid.set(0,0,0);
	vel.set(0,0,0);
}

void ofxUser::setup(int _userId, Perspective * _perspective){
	userId	= _userId;
	perspective = _perspective;
	
	reIni();
}

void ofxUser::update(){
	if(isIni){
		vel			= centroid - lastCentroid;
		
		velF	= vel.length();
		vel		= vel.getNormalized();
		
		lastCentroid = centroid;
		
		if (isActive){
			if(bones){
				bonesVel	= bonesPoints - bonesLastPoints;
			
				bonesVelF = bonesVel.length();
				//bonesVel.normalize();
		
				bonesLastPoints = bonesPoints;
	
				ofVec3f min, max;
				min.set(1000,1000,0);
				max.set(0,0,10000);	
		
				for(int i = 0 ; i < 15 ; i++)
					if( bonesPoints.bone[i]->x < min.x)
						min.x = bonesPoints.bone[i]->x;
		
				for(int i = 0 ; i < 15 ; i++)
					if( bonesPoints.bone[i]->y < min.y)
						min.y = bonesPoints.bone[i]->y;
		
				for(int i = 0 ; i < 15 ; i++)
					if( bonesPoints.bone[i]->z > min.z)
						min.z = bonesPoints.bone[i]->z;
		
				for(int i = 0 ; i < 15 ; i++)
					if( bonesPoints.bone[i]->x > max.x)
						max.x = bonesPoints.bone[i]->x;
			
				for(int i = 0 ; i < 15 ; i++)
					if( bonesPoints.bone[i]->y > max.y)
						max.y = bonesPoints.bone[i]->y;
		
				for(int i = 0 ; i < 15 ; i++)
					if( bonesPoints.bone[i]->z < max.z)
						max.z = bonesPoints.bone[i]->z;
		
				area.set(min,max);
			}
		}
		
		if ( velF > 0.5 ){
			isIdle = false;
			idleTime == 0;
		} else {
			isIdle = true;
			idleTime++;
		}

		if (idleTime > 100)
			isActive = bones = false;
	}
}
