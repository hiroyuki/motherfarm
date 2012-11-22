//
//  GradualChangeGrayState.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_GradualChangeGrayState_h
#define motherfarmLED_GradualChangeGrayState_h
#include "BaseState.h"

class GradualChangeGrayState : public BaseState
{
public:
    int longestLen;
    float compressw, compressh;
    
    GradualChangeGrayState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("GradualChangeGrayState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        compressh = compressw = 0.1f;
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
        sharedData->doNoise = 0;
    }
    
    void update()
    {
        BaseState::update();
        int t = ofGetElapsedTimeMillis() * 0.1f;
        float compw = 0, comph = 0;
        int lastupdatex, lastupdatey;
        for( int i = 0; i < SVG_WIDTH; i++)
        {
            if ( compw - floor( compw ) < compressw)
            {
                for( int j = 0; j < SVG_HEIGHT; j++)
                {
                    if ( comph - floor(comph) < compressh)
                    {
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4] = ofNoise(i*0.005+t*0.0002, j*0.005+t*0.002) * 50.f*alpha;
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+1] = colorPixels->getPixels()[(i+j*SVG_WIDTH)*4];
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+2] = colorPixels->getPixels()[(i+j*SVG_WIDTH)*4];
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+3] = 255*alpha;
                        lastupdatey = j;
                    }
                    else
                    {
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4] = colorPixels->getPixels()[(i+lastupdatey*SVG_WIDTH)*4];
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+1] = colorPixels->getPixels()[(i+lastupdatey*SVG_WIDTH)*4];
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+2] = colorPixels->getPixels()[(i+lastupdatey*SVG_WIDTH)*4];
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+3] = 255*alpha;
                    }
                }
                comph += compressh;
                lastupdatex = i;
            }
            else
            {
                for( int j = 0; j < SVG_HEIGHT; j++)
                {
                    colorPixels->getPixels()[(i+j*SVG_WIDTH)*4] =
                    colorPixels->getPixels()[(lastupdatex+j*SVG_WIDTH)*4];
                    colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+1] =
                    colorPixels->getPixels()[(lastupdatex+j*SVG_WIDTH)*4+1];
                    colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+2] =
                    colorPixels->getPixels()[(lastupdatex+j*SVG_WIDTH)*4+2];
                    colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+3] = 255*alpha;
                }
            }
            comph = 0;
            compw += compressw;
        }
        tex->loadData(*colorPixels);
        fbo->begin();
        ofClear(0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        tex->draw(0, 0);
        ofDisableBlendMode();
        fbo->end();
    }
    
    void draw()
    {
        BaseState::draw();
        
    }
    
    string getName()
    {
        return "GradualChangeGrayState";
    }
};


#endif
