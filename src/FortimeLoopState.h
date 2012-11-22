//
//  FortimeLoopState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_FortimeLoopState_h
#define motherfarmLED_FortimeLoopState_h
#include "BaseState.h"
enum STATUS_COLOR{
    CHNAGING, WAITING
};

class pixzel
{
public:
    
    //500ms timing 13578 -- 8white
    int colors[8] = {0x552345, 0x246733, 0x234473, 0x726522,
                        0x842200, 0x996733, 0x114473, 0x72656f};
    ofPoint pos;
    int size;
    ofColor color;
    float changingCursol;
    STATUS_COLOR status;
    int startms;
    int colorIndex;
    int colorCounter;
    ofColor curColor, nextColor;
    int delay = 0;
    
    void setup(int x, int y, int size)
    {
        pos.x = x;
        pos.y = y;
        changingCursol = 0;
        this->size = size;
        colorIndex = (int)ofRandom(4);
        colorCounter = 0;
        color.setHex(colors[ colorIndex ]);
        curColor = color;
    }
    
    void start()
    {
        startms = ofGetElapsedTimeMillis();
        status = WAITING;
        colorCounter = 0;
    }
    
    void update(bool doBang)
    {
        if ( status == WAITING)
        {
            if (doBang)
            {
                colorCounter++;
                if ( colorCounter == 8)
                {
//                    if ( delay == 0) delay = ofGetElapsedTimeMillis();
//                    else if ( ofGetElapsedTimeMillis() - delay > 200 )
                    {
                        status = CHNAGING;
                        colorCounter = 0;
                        colorIndex++;
                        colorIndex %= 8;
                        nextColor.setHex(colors[colorIndex]);
                        curColor.setHex(0x999999);
                        color = curColor ;
                        startms = ofGetElapsedTimeMillis();
                        delay = 0;
                    }
                }
                else //if ( colorCounter < 7)
                {
                    status = CHNAGING;
                    colorIndex++;
                    colorIndex %= 8;
                    nextColor.setHex(colors[colorIndex]);
                    startms = ofGetElapsedTimeMillis();
                }
            }
        
        }
        else
        {
            if ( curColor != ofColor::white)
            {
                changingCursol = ofClamp((ofGetElapsedTimeMillis() - startms) / 400.f, 0.f, 1.f);
            }
            else 
            {
                changingCursol = ofClamp((ofGetElapsedTimeMillis() - startms) / 400.f, 0.f, 1.f);
            }
            curColor.r = color.r + (nextColor.r - color.r) * changingCursol;
            curColor.g = color.g + (nextColor.g - color.g) * changingCursol;
            curColor.b = color.b + (nextColor.b - color.b) * changingCursol;
//            if ( changingCursol > 0.5)
            if ( changingCursol > 0.99)
            {
                status = WAITING;
                curColor = color = nextColor;
                changingCursol = 1;
            }
        }
    }
    
    void draw()
    {
        ofSetColor(curColor);
        ofRect(pos.x, pos.y, size, size);
    }
};

class FortimeLoopState : public BaseState
{
public:
    vector<pixzel > rects;
    FortimeLoopState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("FortimeLoopState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
        for( int i = 0; i < SVG_WIDTH; i+=5)
        {
            for( int j = 0; j < SVG_HEIGHT; j += 5 )
            {
                rects.push_back( pixzel() );
                rects.back().setup( i, j, 5);
            }
        }
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
        sharedData->doNoise = 0;
        for( int i= 0; i < rects.size();i++)
        {
            rects[i].start();
        }
    }
    
    void update()
    {
        BaseState::update();
        
        fbo->begin();
        ofClear(0);
        {
            //            sharedData->noiseAlpha = sharedData->doNoise == 1 ? ofLerp(sharedData->noiseAlpha, 1.f, 0.1)
            //            :ofLerp(sharedData->noiseAlpha, 0.f, 0.01);
            for( int i = 0; i < rects.size(); i++ )
            {
                rects[i].update(sharedData->gotBang500);
                rects[i].draw();
            }
        }
        fbo->end();
    }
    
    void draw()
    {
        BaseState::draw();
    }
    string getName()
    {
        return "FortimeLoopState";
    }
};
#endif
