//
//  Sockets.cpp
//  PunchOut
//
//  Created by Joseph Chow on 1/31/14.
//
//

#include "Sockets.h"
#include "PunchParser.h"
using namespace std;
using namespace ci;
Sockets::Sockets(string socket_url,bool socket_debug){
    url = socket_url;
    socketDebug = socket_debug;
    
    socket_client.connect(url);
    
    socket_client.addConnectCallback( &Sockets::onConnect, this );
	socket_client.addDisconnectCallback( &Sockets::onDisconnect, this );
	socket_client.addErrorCallback( &Sockets::onError, this );
	socket_client.addInterruptCallback( &Sockets::onInterrupt, this );
	socket_client.addPingCallback( &Sockets::onPing, this );
	socket_client.addReadCallback( &Sockets::onMessage, this );
}


void Sockets::poll(){
    socket_client.poll();
}

void Sockets::onConnect()
{
	status = "Connected";
    if(socketDebug){
        app::console()<<"Socket has established connected to :"<<url<<endl;
    }
}

void Sockets::onDisconnect()
{
	status = "Disconnected";
    if(socketDebug){
        app::console()<<"Socket has disconnected from :"<<url<<endl;
    }
}

void Sockets::onError( string err )
{
	status = "Error";
	if ( !err.empty() ) {
		status += ": " + err;
        if(socketDebug){
            app::console()<<"Socket has errored : "<<err<<endl;
        }
	}
}

void Sockets::onInterrupt()
{
	status = "Interrupted";
    
    if(socketDebug){
        app::console()<<"Socket connection has been interrupted"<<endl;
    }
}

void Sockets::onPing( string msg )
{
	status = "Ponged";
	if ( !msg.empty() ) {
		status += ": " + msg;
        
        if(socketDebug){
            app::console()<<"Socket has received msg in ping:"<<msg<<endl;
        }
	}
}

void Sockets::onMessage( string msg )
{
	status = "Read";
	if ( !msg.empty() ) {
		status += ": " + msg;
        if(socketDebug){
            app::console()<<"Socket has received msg in read :"<<msg<<endl;
        }
        
        PunchParser::parseHands(msg);
	}
}

