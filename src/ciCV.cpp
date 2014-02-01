//
//  ciCV.cpp
//  PunchOut
//
//  Created by Joseph Chow on 1/30/14.
//
//

#include "ciCV.h"
using namespace ci;
using namespace std;

ciCV::ciCV(int vidWidth,int vidHeight,bool debug){
    width = vidWidth;
    height = vidHeight;
    
    debugging = debug;
    thresholdLevel = 38.0f;
    blobMin = 7.78f;
    blobMax = 78.0f;
}

void ciCV::setup(){
    cam = Capture::create( width,height );
    cam->start();
    
    if(debugging){
        mParams = params::InterfaceGl( "Hand Tracking", Vec2i( 10, 10 ) );
        mParams.addParam( "Threshold", &thresholdLevel, "min=0.0 max=255.0 step=1.0 keyIncr=s keyDecr=w" );
        mParams.addParam( "Blob Minimum Radius", &blobMin, "min=1.0 max=200.0 step=1.0 keyIncr=e keyDecr=d" );
        mParams.addParam( "Blob Maximum Radius", &blobMax, "min=1.0 max=200.0 step=1.0 keyIncr=r keyDecr=f" );
    
        
    }
}

void ciCV::drawContours(){
  
    cv::Scalar colors[3];
    colors[0] = cv::Scalar(255, 0, 0);
    colors[1] = cv::Scalar(0, 255, 0);
    colors[2] = cv::Scalar(0, 0, 255);
    for(int i = 0;i<contours.size();++i){
       cv::drawContours(input,contours,i,colors[i % 3]);
    }
}

void ciCV::update(){
    if( cam && cam->checkNewFrame() ) {
        frame = cam->getSurface();
        cv::Mat thresholded;
        cv::Mat input(toOcv(Channel8u(frame)));
        
        //run the threshold
        cv::threshold(input,thresholded,thresholdLevel,255,CV_THRESH_BINARY);
        
        
        cv::findContours(thresholded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        cv::cvtColor( thresholded, outputDebug, CV_GRAY2RGB );
        
        for (vector<vector<cv::Point> >::iterator it=contours.begin() ; it < contours.end(); it++ ){
 
            // center abd radius for current blob
            cv::Point2f center;
            float radius;
            // convert the cuntour point to a matrix
            vector<cv::Point> pts = *it;
            cv::Mat pointsMatrix = cv::Mat(pts);
            // pass to min enclosing circle to make the blob
            cv::minEnclosingCircle(pointsMatrix, center, radius);
            
            cv::Scalar color( 0, 255, 0 );
           
            if (radius > blobMin && radius < blobMax) {
                targetposition.x = center.x;
                targetposition.y = center.y;
                targetposition.z = 0;
            }
        }
        
        cvTexture = gl::Texture(fromOcv(outputDebug));
        
	}
}

void ciCV::draw(){
    gl::drawSolidCircle(Vec2f(targetposition.x,targetposition.y),30);
}