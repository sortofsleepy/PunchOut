//
//  PunchParser.h
//  PunchOut
//
//  Created by Joseph Chow on 1/31/14.
//
//
/**
 Parses and encodes data to be sent to 
 Server
*/
#ifndef PunchOut_PunchParser_h
#define PunchOut_PunchParser_h

#include "cinder/Json.h"
#include "cinder/app/AppNative.h"
#include <string>
#include <vector>
#include "cinder/Vector.h"

using namespace ci;
using namespace std;

class PunchParser{
    
public:
    PunchParser(){}
    
    /**
     Parses out the type of message thats being sent.
     */
    static string parse(string jsonstring){
     JsonTree parser;
        parser = JsonTree(jsonstring);
        string type;
        for( JsonTree::ConstIter cIt = parser.begin(); cIt != parser.end(); ++cIt )
        {
            if(cIt->getKey() == "userid"){
                type = "id";
            }
            
            if(cIt->getKey() == "hands"){
                type = "hands";
            }
            
            if(cIt->getKey() == "head"){
                type = "head";
            }
            
        }
        return type;
    }
    
    /**
     Parses out the User's id
    */
    static string parseID(string jsonstring){
        JsonTree parser;
        parser = JsonTree(jsonstring);
        string user_id;
        for( JsonTree::ConstIter cIt = parser.begin(); cIt != parser.end(); ++cIt )
        {
            user_id = cIt->getValue();
            
        }
       
        return user_id;
    }
    
    
    /**
     Parses out the head data
    */
    static Vec3f parseHead(string jsonstring){
        JsonTree parser(jsonstring);
        
        Vec3f data;
        
        JsonTree::ConstIter it;
        for(it = parser.begin();it != parser.end();++it){
            if(it->getKey() == "x"){
                data.x = stof(it->getValue());
            }
            
            if(it->getKey() == "y"){
                data.y = stof(it->getValue());
            }
            
            if(it->getKey() == "z"){
                data.z = stof(it->getValue());
            }
        }
        
        return data;
    }
    
    /**
     Parses the hand data
     */
    static std::vector<Vec3f> parseHands(string jsonstring){
        JsonTree parser(jsonstring);
        
        /**
         left hand will occupy 0 index.
         right hand will occupy 1 index;
        */
        vector<Vec3f> data;
        
        //vec3f data
        Vec3f ldata,rdata;
        
        //will parse left and right hand data
        JsonTree left;
        JsonTree right;
        
        //loop through to get indices first
        JsonTree::ConstIter it;
        for(it = parser.begin();it != parser.end();++it){
            if(it->getKey() == "left"){
        
                ldata = Vec3f(stof((*it)["x"].getValue()),stof((*it)["y"].getValue()),stof((*it)["z"].getValue()));
            }
            
            if(it->getKey() == "right"){
                rdata = Vec3f(stof((*it)["x"].getValue()),stof((*it)["y"].getValue()),stof((*it)["z"].getValue()));
            }
        }
        
        return data;

    }
};

#endif
