//
//  DanganState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_DanganState_h
#define motherfarmLED_DanganState_h
#include "CircleToOutState.h"
#include "DanganCircle.h"

class DanganState : public CircleToOutState
{
public:
    vector<DanganCircle> circles;
    DanganState() : CircleToOutState()
    {
    }
    
    void setSharedData( SharedData* sharedData)
    {
        this->sharedData = sharedData;
        gui.addToggle("DanganState", isActive);
    }
    
    void setup()
    {
        CircleToCenterState::setup();
        toCenter = false;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
    }
    
    void update()
    {
        BaseState::update();
        for( int i = 0; i < circles.size(); i++)
        {
            circles[ i ].update();
            if ( circles[ i ].status == STATUS_HIDE)
            {
                circles[ i ].init(SVG_WIDTH, SVG_HEIGHT, longestLen);
                if ( circles.size() < MAX_CIRCLE )
                {
                    circles.push_back(DanganCircle(circles.size(), toCenter));
                    circles.back().init(SVG_WIDTH, SVG_HEIGHT, longestLen);
                }
                else
                {
                    circles[i].angleRad = (circles.back().no + 1 )*0.1f;
                }
            }
        }
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo->begin();
        ofSetColor(240.f*alpha, 240.f*alpha, 240.f*alpha,240.f*alpha);
        fbo->draw(0, 0);
        ofSetColor(255, 255, 255);
        //        glLineWidth(3);
        glPointSize(2);
        
        ofPushMatrix();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for( int j = 0; j < circles.size(); j++)
        {
            circles[j].draw();
        }
        ofPopMatrix();
        ofDisableBlendMode();
        fbo->end();
    }
    
    void stateEnter()
    {
        while( circles.size() < MAX_CIRCLE / 2 )
        {
            circles.push_back(DanganCircle(circles.size(), toCenter));
            circles.back().init(SVG_WIDTH, SVG_HEIGHT, longestLen);
        }
        BaseState::stateEnter();
        toCenter = false;
    }
    
    string getName()
    {
        return "DanganState";
    }
};


#endif
