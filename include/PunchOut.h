//
//  PunchOut.h
//  PunchOut
//
//  Created by Joseph Chow on 1/30/14.
//
//

#ifndef PunchOut_PunchOut_h
#define PunchOut_PunchOut_h

#include "LeapApp.h"
#include "Sockets.h"
#include "json/json.h"
#include "json/writer.h"
#include "cinder/Vector.h"
#include "PunchParser.h"


class PunchOut : public LeapApp{
    
    //position of the users head
    ci::Vec3f headpos_;
    
    //positions of the hands
    ci::Vec3f lefthand,righthand;
    
    //position of the opponent's head
    ci::Vec3f o_headpos;
    ci::Vec3f o_lefthand,o_righthand;
    
    //Socket connection
    Sockets * client;

    
public:
    ci::Vec3f headpos;
    PunchOut();

    void setup();
    void update();
    void onFrame(Leap::Frame frame);
    
    void onMessage(std::string msg);
    
    //our assigned ID for this instance.
    int _id;
    
    //when we receive data on opposition "head"
    void onOpponentHead();
    
    //when we receive data on opposition hand position
    void onHandPosition();
    
    /**
     When we send a message to server.
    */
    void onPing(std::string msg);
    
    void onRead(std::string msg);
    
    void setID();
    
    void onConnect();
    
    
    /** MESSAGE PASSING W/ SOCKET SERVER */
    void sendDeleteMessage();
    void sendHeadUpdate();
    void sendHandUpdate();
};

#endif
