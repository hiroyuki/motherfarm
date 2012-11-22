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
#include "WindSensorManager.h"
#ifdef RISE
#define SVG_WIDTH 590
#define SVG_HEIGHT 580
#else
#define SVG_WIDTH 450
#define SVG_HEIGHT 280
#endif
#define MAX_STARS 4
class FarmEventData
{
public:
    string stateName;
    string eventName;
    int bActive;
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
private:
    float angle;
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
    int testEnttec;
    int testLed;
    int mode;
    int dataManualEnntecNo;
    ofPixels extraLED;
    vector< CircleSeed> stars;
    int doNoise;
    float noiseAlpha;
    int startMs;
    float curveA, curveB, treeAlpha;
    bool gotBang4000;
    bool gotBang2000;
    bool gotBang1000;
    bool gotBang500;
    bool gotBang250;
    
    bool bFillCircleState;
    bool bMocieState;
    
    bool bDefaultBlend, bSRandomState;
    
    WindSensorManager windManager;
    
    SharedData():curveA(0), curveB(0)
    {}
    
    void setup()
    {
        cout << "sharedData setup" << endl;
        tex = new ofTexture();
        tex->allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
        colorPixels = new ofPixels();
        colorPixels->allocate(SVG_WIDTH, SVG_HEIGHT, OF_IMAGE_COLOR_ALPHA);
        location.setup(colorPixels);
        recv.setup(12345);
        sender.setup("127.0.0.1", 12347);
        ofxOscMessage message;
        message.setAddress("/clear");
        sender.sendMessage(message);
        show2D = false;
        showTex  = true;
        mode = TEST_NONE;
        extraLED.allocate(4, 2, 3);
        bDefaultBlend = false;
        doNoise = false;
        dataManualEnntecNo = -1;
        noiseAlpha = 0;
        showParse = false;
        for ( int i= 0; i < MAX_STARS; i++ )
        {
            stars.push_back(CircleSeed());
            stars.back().init(SVG_WIDTH, SVG_HEIGHT);
        }
        windManager.setup();
    }
    
    
    void update()
    {
//        cout << "shareddata update" <<endl;

        gotBang4000 = false;
        gotBang2000 = false;
        gotBang1000 = false;
        gotBang500 = false;
        gotBang250 = false;
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
//            cout << message.getAddress() << endl;
            if ( message.getAddress() == "/duration/info")
            {
                cout << "send message" << endl;
                ofxOscMessage m;
                m.setAddress("/play");
                startMs = ofGetElapsedTimeMillis();
                sender.sendMessage(m);
            }
            if ( message.getAddress() == "/tempo4000" )
            {
                gotBang4000 = true;
            }
            if ( message.getAddress() == "/tempo2000" )
            {
                gotBang2000 = true;
            }
            if ( message.getAddress() == "/tempo1000" )
            {
                gotBang1000 = true;
            }
            if ( message.getAddress() == "/tempo500")
            {
                gotBang500 = true;
            }
            if ( message.getAddress() == "/tempo250")
            {
                gotBang250 = true;
            }
            if ( message.getAddress() == "/treeAlpha")
            {
                treeAlpha = message.getArgAsFloat(0);
            }
            if ( message.getAddress().substr(0, 7) == "/state ")
            {
                string stateName = message.getAddress().substr(7);
                if ( stateName == "FillCircleState")
                {
                    bFillCircleState = message.getArgAsInt32(0) == 1;
                }
                if ( stateName == "MovieState")
                {
                    bMocieState = message.getArgAsInt32(0) == 1;
                }
                if ( stateName == "SRandomNoiseState")
                {
                    bSRandomState = message.getArgAsInt32(0) == 1;
                }
                changeState(stateName, message.getArgAsInt32(0));
            }
            if ( message.getAddress() == "/curveA" )
            {
                curveA = message.getArgAsFloat(0);
            }
            if ( message.getAddress() == "/curveB" )
            {
                curveB = message.getArgAsFloat(0);
            }
        }
    
        if ( bFillCircleState )
        {
            location.treeModel->mode_tree = TREE_BPM2000;
            location.treeModel->mode_beam = BEAM_ROUND4000;
        }
        else if ( bMocieState )
        {
            location.treeModel->mode_tree = TREE_BPM2000;
            location.treeModel->mode_beam = BEAM_ROUND1000;
        }
        else if ( bSRandomState )
        {
            location.treeModel->mode_tree = TREE_NOISE;
            location.treeModel->mode_beam = TREE_NONE;
        }
        else
        {
            location.treeModel->mode_tree = TREE_NONE;
        }
        location.update(gotBang4000, gotBang2000, gotBang1000, gotBang500, treeAlpha);
        windManager.update();
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
            stars[i].update(gotBang2000);
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
    
    void changeState( string stateName, int bActive )
    {
        {
            dt.eventName = "changeState";
            dt.stateName = stateName;
            dt.bActive = bActive;
            ofNotifyEvent(event.farmEvent, dt, this);
        }
    }
    
    void sendExit()
    {
        ofxOscMessage m;
        m.setAddress("/exit");
        sender.sendMessage(m);
    }
    
    void sendStart()
    {
        ofxOscMessage m;
        m.setAddress("/play");
        sender.sendMessage(m);
    }
    
    int getWindSpeed()
    {
        return 1100 - ofClamp(windManager.getSpeed(), 0, 1000);
    }
    
    float getAngle()
    {
        return windManager.getAngle();
    }
};
#endif
