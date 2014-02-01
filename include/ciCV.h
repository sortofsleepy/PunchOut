//
//  ciCV.h
//  PunchOut
//
//  Created by Joseph Chow on 1/30/14.
//
//

#ifndef PunchOut_ciCV_h
#define PunchOut_ciCV_h

#include "cinder/Capture.h"
#include "CinderOpenCv.h"
#include "cinder/params/Params.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include <vector>
class ciCV{
    int width,height;
    
    ci::CaptureRef cam;
    ci::Surface8u frame;
    
    cinder::params::InterfaceGl mParams;
    
    bool debugging;
    float blobMin;
    float blobMax;
    float thresholdLevel;
    
    //used for debugging so we can draw onto it.
    cv::Mat outputDebug;
    
    ci::gl::Texture cvTexture;
    
    ci::Vec3f targetposition;
    std::vector<std::vector<cv::Point> > contours;


    //image
    cv::Mat input;
public:
    ciCV(int vidWidth=640,int vidHeight=480,bool debug=false);
    
    void setup();
    void drawContours();
    void update();
    void draw();

};


#endif
