//
//  Sockets.h
//  PunchOut
//
//  Created by Joseph Chow on 1/31/14.
//
//

#ifndef PunchOut_Sockets_h
#define PunchOut_Sockets_h

#include <string>
#include "WebSocketClient.h"
#include "cinder/app/AppNative.h"

class Sockets{
    WebSocketClient socket_client;
    
    std::string url;
    bool socketDebug;
    std::string status;
public:
    Sockets(std::string socket_url="ws://192.168.1.5:8000",bool socket_debug=true);
    WebSocketClient getClient();
    void poll();
    void onConnect();
    void onDisconnect();
    void onError(std::string err);
    void onInterrupt();
    void onPing(std::string msg);
    void onMessage(std::string msg);
    
    
    template<typename T, typename Y>
	inline void addConnectCallback( T callback, Y *callbackObject )
	{
        socket_client.addConnectCallback(callback, callbackObject);
	}
    
    template<typename T, typename Y>
	inline void addReadCallback( T callback, Y *callbackObject )
	{
        socket_client.addReadCallback(callback, callbackObject);
	}
    
    template<typename T>
    void write(T msg){
        socket_client.write(msg);
    }
};

#endif
