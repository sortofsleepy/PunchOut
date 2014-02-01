//
//  LeapApp.cpp
//  LeapMagic
//
//  Created by Joseph Chow on 1/28/14.
//
//

#include "LeapApp.h"
using namespace ci;
using namespace ci::app;
using namespace std;
using namespace Leap;
using namespace Math;
LeapApp::~LeapApp(){}
LeapApp::LeapApp(){
    isConnected = false;
    isFocused = false;
    isInitialized = false;
    hasNewFrame = false;
    isExited = false;
    debug = false;
}


void LeapApp::initLeap(){
    leap = new Leap::Controller(*this);
    
    /**
     For some reason, absolutely needs to be set
     in order to get frame data with most
     recent SDK
     */
    leap->setPolicyFlags( Leap::Controller::PolicyFlag::POLICY_BACKGROUND_FRAMES );
}



void LeapApp::connectEventHandler( const function<void( Leap::Frame )>& eventHandler )
{
    mEventHandler = eventHandler;
}
/**
 Toggles debugging on/off.
*/
void LeapApp::debugMode(){
    if(debug){
        debug = false;
    }else{
        debug = true;
    }
}

void LeapApp::updateLeapData(){
    if(isConnected && isInitialized && hasNewFrame){
        mEventHandler(frame);
        hasNewFrame = false;
    }
}


/*============ START FUNCTIONS ===============*/

/**
 Returns the direction of a swipe gesture along 
 with the hand that did the swipe, if possible.
*/
map<string,Vec3f> LeapApp::swipeDirection(){
    

    map<string,Vec3f> return_data;
    
    const Leap::GestureList& gestures = frame.gestures();
	for ( Leap::GestureList::const_iterator iter = gestures.begin(); iter != gestures.end(); ++iter ) {
		
        const Leap::Gesture& gesture	= *iter;
        Leap::Gesture::Type type		= gesture.type();
        
        if ( type == Leap::Gesture::Type::TYPE_SWIPE ) {
           
            
            
            
           //get reference to swipe gesture data
            const Leap::SwipeGesture& gestureMade = (Leap::SwipeGesture)*iter;
            
            
            map<string,Leap::Hand> hand_data = getHands();
            
            /**
                Loop through hand pointables. 
                Compare with gesture pointable id so we
                can figure out which hand is associated with 
                the gesture.
             */
            if(hand_data.size() > 1){
                Leap::Hand left_hand = hand_data["left"];
                Leap::Hand right_hand = hand_data["right"];
                
                PointableList left_pointables = left_hand.pointables();
                PointableList right_pointables = right_hand.pointables();
                for(int i = 0;i<left_pointables.count();++i){
                    if(left_pointables[i].id() == gestureMade.pointable().id()){
                        Leap::Vector v = gestureMade.direction();
                
                        return_data.insert(pair<string,Vec3f>("left",LeapMath::toVec3f(v)));
                    }

                }
                
                for(int i = 0;i<right_pointables.count();++i){
                    if(right_pointables[i].id() == gestureMade.pointable().id()){
                        Leap::Vector v = gestureMade.direction();
                        
                      
                        return_data.insert(pair<string,Vec3f>("right",LeapMath::toVec3f(v)));
                    }
                    
                }
                
            }else{
                Leap::Hand hand = hand_data["hand"];
                PointableList hand_pointables = hand.pointables();
                
                for(int a = 0;a<hand_pointables.count();++a){
                    Leap::Vector v = gestureMade.direction();
                  
                    return_data.insert(pair<string,Vec3f>("hand", LeapMath::toVec3f(v)));
                }
            }
        
        }

    }
    return return_data;

}

/**
 Returns the distance between your left and 
 right hands.
 TODO figure out if theres a better way of doing this;
*/

float LeapApp::getHandDistance(){
    map<string,Leap::Hand> hands = getHands();
    float distance = 0;
    if(hands.size() > 1){
        Leap::Hand left = hands.at("left");
        Leap::Hand right = hands.at("right");
        
        Leap::Vector leftSphere = left.sphereCenter();
        Vec3f left_sphere = Vec3f(leftSphere.x,leftSphere.y,leftSphere.z);
        
        Leap::Vector rightSphere = right.sphereCenter();
        Vec3f right_sphere = Vec3f(rightSphere.x,rightSphere.y,rightSphere.z);
        
        distance = (left_sphere - right_sphere).length();
        
    }
    return distance;
}




/**
 Returns data on the left and right hands. Or just one
 hand if thats how you roll. 
 
 This is using the built in functionality of
 detecting the leftmost or rightmost hand. 
 Should not be considered super accurate.
 
 TODO figure out a more accurate way of tracking hands.
*/
map<string,Leap::Hand> LeapApp::getHands(){
    Leap::HandList hands = frame.hands();
    map<string,Leap::Hand> handdata;
    
    if(hands.count() > 1){
        Leap::Hand leftmost = hands.leftmost();
        Leap::Hand rightmost = hands.rightmost();
        LeapHand left_hand;
        left_hand.rawhand = hands.leftmost();
    
        
        handdata.insert(pair<string, Leap::Hand>("left",leftmost));
        handdata.insert(pair<string, Leap::Hand>("right",rightmost));
    }else{
        handdata.insert(pair<string, Leap::Hand>("hand",hands[0]));
    }

    return handdata;
}
/**=========== EVENT HANDLERS ==============*/
void LeapApp::enableAllGestures(){
    enableSwipe();
    enableTap();
    enableCircle();
}

void LeapApp::enableSwipe(){
    leap->enableGesture(Gesture::TYPE_SWIPE);
}
void LeapApp::enableTap(){
    leap->enableGesture(Gesture::TYPE_KEY_TAP);
}

void LeapApp::enableCircle(){
    leap->enableGesture(Gesture::TYPE_CIRCLE);
}


/**=========== START LEAP CALLBACKS ==============*/

void LeapApp::onConnect( const Leap::Controller& controller )
{
	isConnected = true;
    if(debug){
        app::console()<<"Leap Connected"<<endl;
    }
}

void LeapApp::onDisconnect( const Leap::Controller& controller )
{
    if(debug){
        app::console()<<"Leap Disconnected"<<endl;
    }
	isConnected = false;
}

void LeapApp::onExit( const Leap::Controller& controller )
{
    
	isExited = true;
}

void LeapApp::onFocusGained( const Leap::Controller& controller )
{
    if(debug){
        app::console()<<"Leap App has focus"<<endl;
    }
	isFocused = true;
}

void LeapApp::onFocusLost( const Leap::Controller& controller )
{
    if(debug){
        app::console()<<"Leap App lost focus"<<endl;
    }
	isFocused = false;
}

void LeapApp::onFrame( const Leap::Controller& controller )
{
    
	if ( !hasNewFrame ) {
		frame = controller.frame();
        prevFrame = controller.frame(1);
		hasNewFrame	= true;
	}
    
}

void LeapApp::onInit( const Leap::Controller& controller )
{
    if(debug){
         app::console()<<"Leap initilaized"<<endl;
    }
	isInitialized = true;
}