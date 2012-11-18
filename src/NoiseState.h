//
//  NoiseState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/15/12.
//
//

#ifndef motherfarmLED_NoiseState_h
#define motherfarmLED_NoiseState_h
#include "CircleStar.h"

class NoiseState : public BaseState
{
public:
    int longestLen;
    float compressw, compressh;
    
    NoiseState(SharedData *sharedData):BaseState(sharedData)
    {
        gui.addToggle("NoiseState", isActive);
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
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4] = ofNoise(i*0.001+t*0.003, j*0.001+t*0.002) * 160.f*alpha;
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+1] = ofNoise(i*0.001-t*0.002, j*0.001+t*0.005) * 160.f*alpha;
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+2] = ofNoise(i*0.001-t*0.004, j*0.001-t*0.001) * 160.f * alpha;
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
        sharedData->drawStars(alpha);
        if ( sharedData->doNoise > 0 )
        {
            sharedData->noiseAlpha = sharedData->doNoise == 1 ? ofLerp(sharedData->noiseAlpha, 1.f, 0.1)
                                                                :ofLerp(sharedData->noiseAlpha, 0.f, 0.01);
            for( int i = 0; i < SVG_WIDTH; i+=15)
            {
                for( int j = 0; j < SVG_HEIGHT; j += 15 )
                {
                    if ( ofRandom(1) > 0.5)
                    {
                        ofSetColor(255, 255, 255, sharedData->noiseAlpha*255);
                        ofRect(i, j, 15,15);
                    }
                }
            }
        }
        ofDisableBlendMode();
        fbo->end();
    }
    
    void draw()
    {
        BaseState::draw();
        
    }
    
    string getName()
    {
        return "NoiseState";
    }
};


#endif
