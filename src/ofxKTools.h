/*
 *  ofxKTools.h
 *  KinectUsers
 *
 *  Created by Patricio Gonzalez Vivo on 16/04/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to 
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

#ifndef OFXKTOOLS
#define OFXKTOOLS

#include "ofMain.h"

typedef struct{
	string	name;
	ofMatrix4x4 mat;
} Perspective;

class BonesF {
public:
	float head, neck, torso, left_shoulder, left_elbow, left_hand, left_hip, left_knee, left_foot, right_shoulder, right_elbow, right_hand, right_hip, right_knee, right_foot;
	float *bone[15];
	
	BonesF(){ 
		bone[0] = &head;
		bone[1] = &neck;
		bone[2] = &torso;
		bone[3] = &left_shoulder;
		bone[4] = &left_elbow;
		bone[5] = &left_hand;
		bone[6] = &left_hip;
		bone[7] = &left_knee;
		bone[8] = &left_foot;
		bone[9] = &right_shoulder;
		bone[10] = &right_elbow;
		bone[11] = &right_hand;
		bone[12] = &right_hip;
		bone[13] = &right_knee;
		bone[14] = &right_foot;
	};
	
	void operator=( const float &b ){ 
		for(int i = 0; i < 15; i++) 
			*bone[i] = b; 
	};
	
	bool operator!=( const float &b ){ 
		bool t;
		
		for(int i = 0; i < 15; i++) 
			t = *bone[i] != b; 
		
		return t;
	};
};

class Bones{
public:
	ofVec3f head, neck, torso, left_shoulder, left_elbow, left_hand, left_hip, left_knee, left_foot, right_shoulder, right_elbow, right_hand, right_hip, right_knee, right_foot;
	ofVec3f *bone[15];
	
	bool		tracked;
	bool		normalized;
	
	Bones(){ 
		bone[0] = &head;
		bone[1] = &neck;
		bone[2] = &torso;
		bone[3] = &left_shoulder;
		bone[4] = &left_elbow;
		bone[5] = &left_hand;
		bone[6] = &left_hip;
		bone[7] = &left_knee;
		bone[8] = &left_foot;
		bone[9] = &right_shoulder;
		bone[10] = &right_elbow;
		bone[11] = &right_hand;
		bone[12] = &right_hip;
		bone[13] = &right_knee;
		bone[14] = &right_foot;
		
		tracked = false;
		normalized = false;
	};
	
	void set( float _x, float _y, float _z ){ 
		for(int i = 0; i < 15; i++) 
			bone[i]->set(_x,_y,_z); 
	};
	
	void operator=( const Bones & b ){ 
		for(int i = 0; i < 15; i++) 
			bone[i]->set(b.bone[i]->x,b.bone[i]->y,b.bone[i]->z);
		
		tracked = b.tracked;
		normalized = b.normalized;
	};
	
	Bones  operator-( const Bones & b ) const{
		Bones bones; 
		for(int i = 0; i < 15; i++) 
			*(bones.bone[i]) = *(bone[i]) - *(b.bone[i]);
		return bones;
	};
	
	
	void transform( ofMatrix4x4 mat ){
		for(int i = 0; i < 15; i++) 
			*bone[i] = *bone[i] * mat;
	};
	
	Bones operator *( const ofMatrix4x4 & mat ) const {
		Bones b;
		for(int i = 0; i < 15; i++) 
			*(b.bone[i]) = *bone[i] * mat;
		return b;
	};
	
	void normalize(){
		for(int i = 0; i < 15; i++) 
			bone[i]->normalize();
		normalized = true;
	};
	
	void screenNormalize(){
		for(int i = 0; i < 15; i++){ 
			bone[i]->x = bone[i]->x / 640;
			bone[i]->y = bone[i]->y / 480;
		}
		normalized = true;
	};
	
	void screenExpand(){
		for(int i = 0; i < 15; i++){ 
			bone[i]->x = bone[i]->x * ofGetWidth();
			bone[i]->y = bone[i]->y * ofGetHeight();
		}
		normalized = false;
	};
	
	BonesF length(){
		BonesF bonesF;
		for(int i = 0; i < 15; i++) 
			*(bonesF.bone[i]) = bone[i]->length();
		return bonesF;
	};
	
	void draw(){
		if (tracked){
			glBegin(GL_LINES);
			
			glVertex3f(head.x, head.y, 0);
			glVertex3f(neck.x, neck.y, 0);
			//----------------------------------------- 
			glVertex3f(neck.x, neck.y, 0);
			glVertex3f(left_shoulder.x, left_shoulder.y, 0);
			
			glVertex3f(left_shoulder.x, left_shoulder.y, 0);
			glVertex3f(left_elbow.x, left_elbow.y, 0);
			
			glVertex3f(left_elbow.x, left_elbow.y, 0);
			glVertex3f(left_hand.x, left_hand.y, 0);
			//----------------------------------------- 
			glVertex3f(neck.x, neck.y, 0);
			glVertex3f(right_shoulder.x, right_shoulder.y, 0);
			
			glVertex3f(right_shoulder.x, right_shoulder.y, 0);
			glVertex3f(right_elbow.x, right_elbow.y, 0);
			
			glVertex3f(right_elbow.x, right_elbow.y, 0);
			glVertex3f(right_hand.x, right_hand.y, 0);
			//------------------------------------------ 
			glVertex3f(left_shoulder.x, left_shoulder.y, 0);
			glVertex3f(torso.x, torso.y, 0);
			
			glVertex3f(right_shoulder.x, right_shoulder.y, 0);
			glVertex3f(torso.x, torso.y, 0);
			//------------------------------------------ 
			glVertex3f(torso.x, torso.y, 0);
			glVertex3f(left_hip.x, left_hip.y, 0);
			
			glVertex3f(left_hip.x, left_hip.y, 0);
			glVertex3f(left_knee.x, left_knee.y, 0);
			
			glVertex3f(left_knee.x, left_knee.y, 0);
			glVertex3f(left_foot.x, left_foot.y, 0);
			//------------------------------------------ 
			glVertex3f(torso.x, torso.y, 0);
			glVertex3f(right_hip.x, right_hip.y, 0);
			
			glVertex3f(right_hip.x, right_hip.y, 0);
			glVertex3f(right_knee.x, right_knee.y, 0);
			
			glVertex3f(right_knee.x, right_knee.y, 0);
			glVertex3f(right_foot.x, right_foot.y, 0);
			//-------------------------------------------
			//glVertex3f(left_hip.x, left_hip.y, left_hip.z);
			//glVertex3f(right_hip.x, right_hip.y, right_hip.z);
			glEnd();
		}
	};
};

class Area{
public:
	ofVec3f area[2][2][2];
	ofVec3f min, max;
	
	//					
	//    1z				0x0x1 -- 1x0x1
	//   /					 /|		  /|
	//  0-- 1x			  0x0x0 -- 1x0x0
	//  |					| |		 | |
	//  1y					|0x1x1 - |1x1x1
	//						|/		 |/
	//					  0x1x0 -- 1x1x0
	
	Area(){
		set(ofVec3f(0,0,0),ofVec3f(0,0,0));
	};
	
	//    4 --- 5	Min = Punto 0 = [0][0][0]
	//   /|    /|   Max = Punto 7 = [1][1][1]
	//	0 --- 1 | 
	//  | 6 - | 7	ofVec3f area[x][y][z]
	//	|/	  |/
	//  2 --- 3
	
	void set( ofVec3f _min, ofVec3f _max){
		min = _min;
		max = _max;
		
		update();
	}
	
	ofVec3f get(float _x, float _y, float _z){
		ofVec3f p;
		
		p.x = ofMap(_x, 0, 1, min.x, max.x, true);
		p.y = ofMap(_y, 0, 1, min.y, max.y, true);
		p.z = ofMap(_y, 0, 1, min.z, max.z, true);
		
		return p;
	};
	
	
	void operator=( const Area & a ){ 
		min = a.min;
		max = a.max;
		
		update();
	};
	
	void transform( ofMatrix4x4 mat ){
		min = min * mat;
		max = max * mat;
		
		update();
	};
	
	Area operator *( const ofMatrix4x4 mat ) const{
		Area a;
		a.min = min * mat;
		a.max = max * mat;
		
		a.update();
		return a;
	};
	
	bool check2D( float _x, float _y){
		check2D(ofVec2f(_x,_y));
	}
	
	bool check2D(ofVec2f p){
		check2D(ofVec3f(p.x,p.y,0));
	}
	
	bool check2D(ofVec3f p){
		if ((p.x >= min.x) && (p.x <= max.x) &&
			(p.y >= min.y) && (p.y <= max.y) )
			return true;
		else 
			return false;
	}
	
	bool check2D( float _x, float _y, float _z){
		check3D( ofVec3f(_x,_y,_z));
	}
	
	bool check3D( ofVec3f p){
		if ((p.x >= min.x) && (p.x <= max.x) &&
			(p.y >= min.y) && (p.y <= max.y) &&
			(p.z >= min.z) && (p.z <= max.z) )
			return true;
		else
			return false;
	}
	
	void draw(){
		ofPushMatrix();
		glBegin(GL_LINES);
		
		glVertex3f(area[0][0][0].x, area[0][0][0].y, 0);
		glVertex3f(area[1][0][0].x, area[1][0][0].y, 0);
		
		glVertex3f(area[0][0][0].x, area[0][0][0].y, 0);
		glVertex3f(area[0][1][0].x, area[0][1][0].y, 0);
		
		glVertex3f(area[0][0][0].x, area[0][0][0].y, 0);
		glVertex3f(area[0][0][1].x, area[0][0][1].y, 0);
		
		glVertex3f(area[1][1][1].x, area[1][1][1].y, 0);
		glVertex3f(area[1][1][0].x, area[1][1][0].y, 0);
		
		glVertex3f(area[1][1][1].x, area[1][1][1].y, 0);
		glVertex3f(area[1][0][1].x, area[1][0][1].y, 0);
		
		glVertex3f(area[1][1][1].x, area[1][1][1].y, 0);
		glVertex3f(area[0][1][1].x, area[0][1][1].y, 0);
		
		glEnd();
		ofPopMatrix();
	};
	
	//private:
	void update(){
		ofVec3f p[2] = {min, max};
		for (int x = 0; x < 2; x++)
			for (int y = 0; y < 2; y++)
				for (int z = 0; z < 2; z++)
					area[x][y][z].set(p[x].x,p[y].y,p[z].z);
	};
};

typedef struct { 
	int shoulder, hand; 
} Arm;

class Arms {
public:
	Arm		right, left;
	
	Arms(){;};
	
	bool operator==( const Arms& b ){ 
		return (right.shoulder == b.right.shoulder) && (right.hand == b.right.hand) && (left.shoulder == b.left.shoulder) && (left.hand == b.left.hand); 
	};
};

typedef struct {
	Arms	poseDetonator;
	
	string	name;
	int		timerMin;
} Pose;

typedef struct {
	Arms	initPose;
	Arms	finalPose;
	
	string	name;
	int		timerMax;
} Gesture;
#endif