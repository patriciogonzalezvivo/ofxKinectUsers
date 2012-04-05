#include "ofxOpenNIProxy.h"

ofxOpenNIProxy::ofxOpenNIProxy(){
	isMasking = isCloud	= false;
	//filterFactor = 0.1f;
}

void ofxOpenNIProxy::setup(ofxUser *_users){
	users = _users;
	
#ifdef HARDWARE
	hardware.setup();						// libusb direct control of motor, LED and accelerometers
	//hardware.setLedOption(LED_OFF);
#endif	

	context.setup();
	
	depthGenerator.setup(&context);
	//depthGenerator.setDepthColoring(COLORING_GREY);
	
#ifdef IMAGE
	imageGenerator.setup(&context);
#endif
	
	userGenerator.setup(&context);
	//userGenerator.setSmoothing(filterFactor);	// built in openni skeleton smoothing...
	userGenerator.setUseMaskPixels(isMasking);
	userGenerator.setUseCloudPoints(isCloud);
	
	//context.toggleRegisterViewport();
	context.toggleMirror();
}

void ofxOpenNIProxy::setMasking(bool _b){
	isMasking = _b;
	userGenerator.setUseMaskPixels(isMasking);
}

//--------------------------------------------------------------
void ofxOpenNIProxy::update(){
#ifdef HARDWARE
	hardware.update();
#endif	
	
	context.update();
	depthGenerator.update();

#ifdef IMAGE
	imageGenerator.update();
#endif
	
	userGenerator.update();
	updateUsers(MAX_USERS);
}


void ofxOpenNIProxy::updateUsers(int nUsers = MAX_USERS){	
	xn::UserGenerator xnUserG = userGenerator.getXnUserGenerator();
	xn::DepthGenerator xnDepthG = depthGenerator.getXnDepthGenerator();
	XnUserID* xnUsers = new XnUserID[nUsers];
	XnUInt16  found_users = nUsers;
	
	xnUserG.GetUsers(xnUsers, found_users);
	
	for (int i = 0; i < found_users; ++i){
		//------------------ UPDATE CENTROID POSITION
		XnPoint3D center;
		
	    xnUserG.GetCoM(xnUsers[i], center);
        xnDepthG.ConvertRealWorldToProjective(1, &center, &center);
		
		users[i].centroid.set(center.X,center.Y,center.Z);
		
		//------------------ UPDATE MASK PIXELS (if it´s masking)
		if (isMasking)	
			users[i].maskPixels = userGenerator.getUserPixels(i+1);
		
		//------------------ UPDATE BONES (if it´s tracked)
		if(xnUserG.GetSkeletonCap().IsTracking(xnUsers[i])){
			users[i].bones = true;

			users[i].bonesPoints.tracked = true;
			users[i].bonesPoints.normalized = false;
			
			XnSkeletonJointPosition joint;
			XnPoint3D pt[2];
			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_HEAD, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.head.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_NECK, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
			users[i].bonesPoints.neck.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_TORSO, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.torso.set(pt[0].X,pt[0].Y,pt[0].Z);

			//------------------- LEFT
			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_LEFT_SHOULDER, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.left_shoulder.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_LEFT_ELBOW, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.left_elbow.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_LEFT_HAND, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.left_hand.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_LEFT_HIP, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.left_hip.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_LEFT_KNEE, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.left_knee.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_LEFT_FOOT, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.left_foot.set(pt[0].X,pt[0].Y,pt[0].Z);

			//-------------------------	RIGHT

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_RIGHT_SHOULDER, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.right_shoulder.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_RIGHT_ELBOW, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.right_elbow.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_RIGHT_HAND, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.right_hand.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_RIGHT_HIP, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.right_hip.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_RIGHT_KNEE, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			users[i].bonesPoints.right_knee.set(pt[0].X,pt[0].Y,pt[0].Z);

			xnUserG.GetSkeletonCap().GetSkeletonJointPosition(xnUsers[i], XN_SKEL_RIGHT_FOOT, joint);
			pt[0] = joint.position;
			pt[1] = joint.position;
			if(!users[i].bonesPoints.normalized){
				xnDepthG.ConvertRealWorldToProjective(2, pt, pt);
			}
			if ( ( pt[0].X > 0) && (pt[0].Y > 0) && ( pt[0].Y > 0)) 
				users[i].bonesPoints.right_foot.set(pt[0].X,pt[0].Y,pt[0].Z);
		} else
			users[i].bonesPoints.tracked = false;
	}
	
	delete [] xnUsers;
}
