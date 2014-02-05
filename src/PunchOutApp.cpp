#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Sockets.h"
#include "PunchOut.h"
#include "cinder/TriMesh.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class PunchOutApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    CameraPersp			mCam;

    TriMesh mesh;
    Quatf				mSceneRotation;
	Vec3f				mEye, mCenter, mUp;
	float				mDistortionX;
	float				mDistortionY;
	float				mDistortionZ;
	float				mCameraDistance;
	params::InterfaceGl	mParams;

};

void PunchOutApp::setup()
{
    // Position and Color for the four corners
    mesh.appendVertex( Vec3f( 10, 10, -200 ) );
    mesh.appendColorRgb( Color( 1, 0, 0 ) );
    
    mesh.appendVertex( Vec3f( 10, 100, 0 ) );
    mesh.appendColorRgb( Color( 0, 1, 0 ) );
    
    mesh.appendVertex( Vec3f( 40, 100, 0 ) );
    mesh.appendColorRgb( Color( 0, 0, 1 ) );
    
    mesh.appendVertex( Vec3f( 40, 10, -200 ) );
    mesh.appendColorRgb( Color( 0, 0, 0 ) );
    
    // Indices for each of the four corners
    int vIdx0 = mesh.getNumVertices() - 4;
    int vIdx1 = mesh.getNumVertices() - 3;
    int vIdx2 = mesh.getNumVertices() - 2;
    int vIdx3 = mesh.getNumVertices() - 1;
    
    // Two triangles to create our square
    mesh.appendTriangle( vIdx0, vIdx1, vIdx2 );
    mesh.appendTriangle( vIdx0, vIdx2, vIdx3 );
    
    mCenter	= Vec3f( getWindowWidth() * 0.5f, getWindowHeight() * 0.5f, 0.0f );
	mCameraDistance = 350.0f;
	mEye			= Vec3f( 0.0f, 0.0f, mCameraDistance );
	mCenter			= Vec3f::zero();
	mUp				= Vec3f::yAxis();
	mCam.setPerspective( 75.0f, getWindowAspectRatio(), 5.0f, 5000.0f );
    mDistortionX = 0.0f;
    mDistortionY = 0.0f;
    mDistortionZ = 0.0f;
    
    mParams = params::InterfaceGl( "Distorting Cube", Vec2i( 200, 210 ) );
	mParams.addParam( "Scene Rotation", &mSceneRotation, "opened=1" );
	mParams.addParam( "Eye Distance", &mCameraDistance, "min=100.0 max=2000.0 step=50.0 keyIncr=s keyDecr=w" );
 	mParams.addParam( "Distortion X", &mDistortionX, "min=-5.0 max=5.0 step=.2.0 keyIncr=+ keyDecr=-" );
    mParams.addParam( "Distortion Y", &mDistortionY, "min=-5.0 max=5.0 step=.2.0 keyIncr=+ keyDecr=-" );
    mParams.addParam( "Distortion Z", &mDistortionZ, "min=-5.0 max=5.0 step=.2.0 keyIncr=+ keyDecr=-" );
	mParams.addSeparator();
  
   
}

void PunchOutApp::mouseDown( MouseEvent event )
{
 
}

void PunchOutApp::update()
{
    
    mEye	= Vec3f( 0.0f, 0.0f, mCameraDistance );
    mCam.lookAt( mEye, mCenter, mUp );
	gl::setMatrices( mCam );
	gl::rotate( mSceneRotation );
    
    

}

void PunchOutApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::enableDepthRead();
	gl::enableDepthWrite();
    
    gl::draw(mesh);
    
	// DRAW PARAMS WINDOW
	mParams.draw();
    
}

CINDER_APP_NATIVE( PunchOutApp, RendererGl )
