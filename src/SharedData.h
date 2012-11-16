//
//  SharedData.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/4/12.
//
//

#ifndef motherfarmLED_SharedData_h
#define motherfarmLED_SharedData_h

#include "Location.h"
#include "CircleSeed.h"
#ifdef RISE
#define SVG_WIDTH 590
#define SVG_HEIGHT 580
#else
#define SVG_WIDTH 790
#define SVG_HEIGHT 450
#endif
#define MAX_STARS 5
class FarmEventData
{
public:
    int status;
    string eventName;
    string nextState;
    string movName;
};

enum TEST_MODE
{
    TEST_ENTTEC,
    TEST_LED,
    TEST_ECHODATA,
    TEST_NONE
};

class MotherFarmEvent
{
public:
    ofEvent<FarmEventData> farmEvent;
};

class SharedData
{
public:

    Location location;
    ofxOscReceiver recv;
    ofxOscSender sender;
    MotherFarmEvent event;
    FarmEventData dt;
    ofTexture *tex;
    ofPixels * colorPixels;
    string curState;
    bool show2D, showTex, showParse;
    float angle;
    int testEnttec;
    int testLed;
    int mode;
    int dataManualEnntecNo = -1;
    ofPixels extraLED;
    vector< CircleSeed> stars;
    
    bool bDefaultBlend;
    
    void setup()
    {
        tex = new ofTexture();
        tex->allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
        colorPixels = new ofPixels();
        colorPixels->allocate(SVG_WIDTH, SVG_HEIGHT, OF_IMAGE_COLOR_ALPHA);
        location.setup(colorPixels);
        recv.setup(11111);
        sender.setup("127.0.0.1", 10000);
        ofxOscMessage message;
        message.setAddress("/clear");
        sender.sendMessage(message);
        show2D = false;
        showTex  = true;
        mode = TEST_NONE;
        extraLED.allocate(4, 2, 3);
        bDefaultBlend = false;
        for ( int i= 0; i < MAX_STARS; i++ )
        {
            stars.push_back(CircleSeed());
            stars.back().init(SVG_WIDTH, SVG_HEIGHT);
            stars.back().delay += i * 1000;
        }
    }
    
    void update()
    {
//        cout << "shareddata update" <<endl;
        location.update();
        while (recv.hasWaitingMessages()) {
            ofxOscMessage message;
            recv.getNextMessage(&message);
            if ( message.getAddress() == "/lightpos")
            {
                float posx, posy, posz;
                posx = message.getArgAsFloat(0);
                posy = message.getArgAsFloat(1);
                posz = message.getArgAsFloat(2);
                location.changeLightPos(posx, posy, posz);
            }
            if ( message.getAddress() == "/lightColor")
            {
                location.setLight(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2), message.getArgAsFloat(3), message.getArgAsFloat(4), message.getArgAsFloat(5));
            }
            if ( message.getAddress() == "/cameraPos")
            {
                location.setCameraPos(message.getArgAsFloat(0), message.getArgAsFloat(1), message.getArgAsFloat(2));
            }
            
            if ( message.getAddress() == "/state")
            {
                string stateName = message.getArgAsString(0);
                changeState(stateName);
            }
            if (message.getAddress() == "/show2d")
            {
                show2D = message.getArgAsInt32(0) == 1;
            }
            if (message.getAddress() == "/showTex" )
            {
                showTex = message.getArgAsInt32(0) == 1;
            }
            if (message.getAddress() == "/showParse" )
            {
                showParse = message.getArgAsInt32(0) == 1;
            }
            if ( message.getAddress() == "/angle")
            {
                angle = message.getArgAsFloat(0);
            }
            if ( message.getAddress() == "/enttec" )
            {
                mode = TEST_ENTTEC;
                dataManualEnntecNo = testEnttec = ofClamp(message.getArgAsInt32(0), 1, 19);
            }
            if ( message.getAddress() == "/led")
            {
                mode = TEST_LED;
                testEnttec = ofClamp(message.getArgAsInt32(0), 1, 19);
                testLed = ofClamp(message.getArgAsInt32(1), 1, 150);
            }
            if ( message.getAddress() == "/drawEnttec")
            {
                dataManualEnntecNo = ofClamp(message.getArgAsInt32(0), 1, 19);
            }
        }
    }

    void sendDmx()
    {
        location.sendDmx(dataManualEnntecNo);
        dataManualEnntecNo = -1;
    }
    
    Enttec* getEnttec( int enttecNo )
    {
        Enttec *result = NULL;
        for( int i = 0; i < location.dmxs.size(); i++ )
        {
            if ( location.dmxs[i].no == enttecNo )
            {
                result = &location.dmxs[i];
                continue;
            }
        }
        return result;
    }
    
    void drawStars(float alpha)
    {
        for( int i = 0; i < stars.size();i ++)
        {
            stars[i].update();
            stars[i].draw(alpha);
        }
    }
    
    void sendStates( string stateName )
    {
        ofxOscMessage message;
        message.setAddress("/states");
        message.addStringArg(stateName);
        sender.sendMessage(message);
    }
    
    void changeState( string stateName )
    {
        if ( curState != stateName)
        {
            cout << "changeState " << stateName << endl;
            dt.eventName = "changeState";
            curState = dt.nextState = stateName;
            ofNotifyEvent(event.farmEvent, dt, this);
        }
        else
        {
            dt.eventName = "showCur";
            ofNotifyEvent(event.farmEvent, dt, this);
        }
    }
};
#endif
