//
//  WindowSensorManager.h
//  motherfarmLED
//
//  Created by motherfarm on 11/20/12.
//
//

#ifndef motherfarmLED_WindowSensorManager_h
#define motherfarmLED_WindowSensorManager_h
#include "ofxOsc.h"
#define HOST1 
#define HOST2 "192.168.11.202"
#define HOST3 "192.168.11.203"
#define HOST4 "192.168.11.204"

class WindSensorManager
{
public:
    
    ofxOscReceiver recv;
    int checkInterval = 50;
    float angleRad;
    float speed;
    int lastCheckSec;
    int lastCheckMs;
    int tempCounter = 0;
    int windCounter = 0;
    deque<int> windDirection;
    deque<float> windVelocity;
    float temp;
    float curAngle, curSpeed;
    int sendCounter = 0;

    WindSensorManager():checkInterval(300)
    {
        recv.setup(10000);
    }
    
    
    void setup()
    {

    }
    
    
    void update()
    {
        while (recv.hasWaitingMessages()) {
            ofxOscMessage m;
            recv.getNextMessage(&m);
            if (m.getAddress() == "/sensor/direction")
            {
                windDirection.push_back(m.getArgAsInt32(0));
                if( windDirection.size() > 3)
                    windDirection.pop_front();
                getAngle();
            }
            else if ( m.getAddress() == "/sensor/velocity")
            {
                windVelocity.push_back(m.getArgAsFloat(0));
                if( windVelocity.size() > 3)
                    windVelocity.pop_front();
                getSpeed();
            }
            else if ( m.getAddress() == "/sensor/temp")
            {
                temp = m.getArgAsFloat(0);
            }
        }
        
    }
    
    void sendGetDirection()
    {

    }
    
    void sendGetVelocity()
    {

    }
    
    void sendGetTemp()
    {

    }
    
    int getSpeed()
    {
        float total = 0;
        for( int i=0;i < windVelocity.size(); i++)
        {
            total+= windVelocity[i];
        }
        curSpeed = total / (float)windVelocity.size();
        return ofMap(curSpeed, 0, 6.f, 0, 1000.f, true);
    }
    
    float getAngle()
    {
        float avg = 0;
        float total = 0;
        for( int i=0;i < windDirection.size(); i++)
        {
            total+= windDirection[i];
        }
        avg = total / (float)windDirection.size();
        if ( windDirection.size() > 0 )
            curAngle = windDirection.back() / 1024.f * TWO_PI;
        else
            curAngle = 0;
        
        return curAngle/PI * 180.f+180.f;
    }
    
    void debugDraw()
    {
        ofDrawBitmapString("curAngle:" + ofToString(curAngle/PI * 180.f), ofGetWidth() - 200, 330);
        ofDrawBitmapString("curSpeed:" + ofToString(curSpeed), ofGetWidth() - 200, 360);
        ofDrawBitmapString("curTemp:" + ofToString(temp), ofGetWidth() - 200, 390);
    }
};


#endif
