//
//  LeapApp.h
//  LeapMagic
//
//  Created by Joseph Chow on 1/28/14.
//
//

/**
 The basis for a Leap Motion application. 
 Provides the necessary callbacks expected to be used.
 Based on Stephen Schieberl's Cinder Block
 https://github.com/BanTheRewind/Cinder-LeapMotion
 
*/

#ifndef LeapMagic_LeapApp_h
#define LeapMagic_LeapApp_h

#include "Leap.h"
#include "LeapMath.h"
#include "LeapApp.h"
#include "cinder/Vector.h"
#include "cinder/Matrix.h"
#include <vector>
#include <map>

/**
 Defines a hand.
*/
typedef struct{
    ci::Vec3f pos;
    ci::Vec3f vel;
    std::string handName;
    Leap::Hand rawhand;
    int64_t id;
}LeapHand;


class LeapApp : public Leap::Listener{
    //Leap controller instance
    Leap::Controller * leap;
    
    //Hands
    Leap::Hand left_hand,right_hand,hand;
    
    //current frame
    Leap::Frame frame;
    
    //previous frame
    Leap::Frame prevFrame;
    
    std::function<void ( Leap::Frame )>	mEventHandler;
    
public:
    LeapApp();
    ~LeapApp();

    bool isConnected;
    bool isFocused;
    bool isInitialized;
    bool hasNewFrame;
    bool isExited;
    
    /**
     Are we debugging?
     */
    bool debug;
    
    /**
        Initialize Leap
    */
    void initLeap();
    
    
    /**
     Updates frame data coming in from the leap.
    */
    void updateLeapData();
   
    
    //stores the function in the class that subclasses this one that receives
    //the frame data.
    std::function<void ( Leap::Frame )>	eventHandler;
    
    
    /**
     Turns debugging on/off
    */
    void debugMode();
    
    //enables all the gestures
    void enableAllGestures();
    
    //enables swipe gesture only
    void enableSwipe();
    
    //enables tap gesture only
    void enableTap();
    
    //enable circle gesture only
    void enableCircle();
    /*====================== FUNCTIONS =======================*/
    //meant to return the swipe direction of a
    std::map<std::string,ci::Vec3f> swipeDirection();

    float getHandDistance();
    
    int handsCount();
    std::map<std::string,Leap::Hand> getHands();

    /**=========== START LEAP CALLBACKS ==============*/
    virtual void	onConnect( const Leap::Controller& controller );
    virtual void	onDisconnect( const Leap::Controller& controller );
	virtual void	onExit( const Leap::Controller& controller );
    virtual void	onFrame( const Leap::Controller& controller );
	virtual void	onFocusGained( const Leap::Controller& controller );
	virtual void	onFocusLost( const Leap::Controller& controller );
	virtual void	onInit( const Leap::Controller& controller );

    //adds a listener for the subclassing class to link to this one.
    template<typename T, typename Y>
    void	setOnFrame( T eventHandler, Y *obj )
    {
        connectEventHandler( std::bind( eventHandler, obj, std::placeholders::_1 ) );
    }
    /**
     Sets the function in the subclass to receive the onFrame evnet
     */
  	void connectEventHandler( const std::function<void( Leap::Frame )>& eventHandler );


    
};

/**
 Some helpful math utilities.
*/
namespace Math {
    class LeapMath{
    public:
        LeapMath(){}
        //! Converts a native Leap vector into a Cinder one.
        static ci::Vec3f toVec3f( const Leap::Vector& v ){
            return ci::Vec3f( v.x, v.y, v.z );
        }
        
        //! Converts a Cinder vector into a native Leap one.
        static Leap::Vector	toLeapVector( const ci::Vec3f& v ){
            return Leap::Vector( v.x, v.y, v.z );
        }
        
        //! Converts a native Leap 3x3 matrix into a Cinder one.
        ci::Matrix33f	toMatrix33f( const Leap::Matrix& m ){
            cinder::Matrix33f mtx;
            Leap::FloatArray a = m.toArray3x3();
            for ( size_t i = 0; i < 3; ++i ) {
                size_t j = i * 3;
                cinder::Vec3f row( a[ j + 0 ], a[ j + 1 ], a[ j + 2 ] );
                mtx.setRow( i, row );
            }
            return mtx;
        }
        //! Converts a native Leap 4x4 matrix into a Cinder one.
        static ci::Matrix44f	toMatrix44f( const Leap::Matrix& m ){
            cinder::Matrix44f mtx;
            Leap::FloatArray a = m.toArray4x4();
            for ( size_t i = 0; i < 4; ++i ) {
                size_t j = i * 4;
                cinder::Vec4f row( a[ j + 0 ], a[ j + 1 ], a[ j + 2 ], a[ j + 3 ] );
                mtx.setRow( i, row );
            }
            return mtx;
        }
        //! Converts a Cinder 3x3 matrix into a native Leap one.
        static Leap::Matrix	toLeapMatrix( const ci::Matrix33f& m ){
            Leap::Matrix matrix;
            matrix.xBasis = Leap::Vector( m.m00, m.m01, m.m02 );
            matrix.yBasis = Leap::Vector( m.m10, m.m11, m.m12 );
            matrix.zBasis = Leap::Vector( m.m20, m.m21, m.m22 );
            return matrix;
        }
        //! Converts a Cinder 4x4 matrix into a native Leap one.
        static Leap::Matrix	toLeapMatrix( const ci::Matrix44f m ){
            Leap::Matrix matrix;
            matrix.xBasis = Leap::Vector( m.m00, m.m01, m.m02 );
            matrix.yBasis = Leap::Vector( m.m10, m.m11, m.m12 );
            matrix.zBasis = Leap::Vector( m.m20, m.m21, m.m22 );
            matrix.origin = Leap::Vector( m.m30, m.m31, m.m32 );
            return matrix;
        }
    };
    
}








#endif
