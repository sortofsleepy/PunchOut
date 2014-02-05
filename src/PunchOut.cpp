//
//  PunchOut.cpp
//  PunchOut
//
//  Created by Joseph Chow on 1/30/14.
//
//

#include "PunchOut.h"

using namespace std;
using namespace ci;

PunchOut::PunchOut(){}
void PunchOut::setup(){
    //debug mode for wrapper
    debugMode();
    
    //start Leap Motion
    initLeap();
    
    //enable all of the gestures.
    enableAllGestures();
    
    //set the callback function to run the app
    setOnFrame(&PunchOut::onFrame, this);

    client = new Sockets();
    //hook into the Socket instance to receive messages
    //from server.
    client->addReadCallback(&PunchOut::onMessage,this);
    
    client->addConnectCallback(&PunchOut::onConnect,this);
}

void PunchOut::onConnect(){
    
}

/**
 This parses out the message when we get
 something back from the server
*/
void PunchOut::onMessage(std::string msg){
    string type = PunchParser::parse(msg);

    /**
     If we get a ID type back, that
     means we need to store the ID of 
     this user.
    */
    if(type == "id"){
        _id = stof(PunchParser::parseID(msg));
    }
    
    /**
     If we're getting data back from
     the server and it happens we need to 
     update the opponent's position
     */
    if(type == "opponentupdate"){
        
    }
}


void PunchOut::update(){
    //make sure the instance's data is kept up to date
    //so we can use the class's functionality
    updateLeapData();
    
    //poll socket server for new info
    client->poll();

}

/**
 The "draw" of the app.
 The frame continues to be passed back for 
 convenience but you technically won't have to 
 make use of it in the future.
*/
void PunchOut::onFrame(Leap::Frame frame){
    
}


void PunchOut::onPing(string msg){
    
}

void PunchOut::onRead(string msg){
    
}

/**====== MESSAGE PASSING W/ SOCKET SERVER ===========*/

/**
 Sends a message to the server telling it to delete this
 user.
*/
void PunchOut::sendDeleteMessage(){
    Json::Value event;
    event["id"] = _id;
    event["event"]["type"] = "delete";
    event["event"]["value"] = false;
    
    client->write(event.toStyledString());
    
}
/**
 sends a user to the server about the user's
 head.
*/
void PunchOut::sendHeadUpdate(){
    Json::Value event;
    Json::Value vec(Json::arrayValue);
    vec.append(Json::Value(headpos.x));
    vec.append(Json::Value(headpos.y));
    vec.append(Json::Value(headpos.z));
    
    event["id"] = _id;
    event["event"]["type"] = "headupdate";
    event["event"]["value"] = vec;
    
    client->write(event.toStyledString());
}
/**
 Sends a update to the server about this user's
 hands.
*/
void PunchOut::sendHandUpdate(){
    Json::Value event;
    
    Json::Value left(Json::arrayValue);
    left.append(Json::Value(lefthand.x));
    left.append(Json::Value(lefthand.y));
    left.append(Json::Value(lefthand.z));
    
    
    Json::Value right(Json::arrayValue);
    right.append(Json::Value(righthand.x));
    right.append(Json::Value(righthand.y));
    right.append(Json::Value(righthand.z));
    
    event["id"] = _id;
    event["event"]["type"] = "headupdate";
    event["event"]["value"]["left"] = left;
    event["event"]["value"]["right"] = right;
    
   client->write(event.toStyledString());
    
}