#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Sockets.h"
#include "PunchOut.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class PunchOutApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    PunchOut p;
  //  Sockets * s;
};

void PunchOutApp::setup()
{
   // s = new Sockets();
    p.setup();
    p.headpos = Vec3f(100,100,100);
}

void PunchOutApp::mouseDown( MouseEvent event )
{
    p.sendHeadUpdate();
}

void PunchOutApp::update()
{
    
    //s->poll();
    p.update();
   
}

void PunchOutApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
   // v.draw();
    
}

CINDER_APP_NATIVE( PunchOutApp, RendererGl )
