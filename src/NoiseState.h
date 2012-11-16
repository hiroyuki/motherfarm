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
    ofTexture *tex;
    ofFbo fbo;
    ofPixels * colorPixels;
    int longestLen;
    ofPixels *smallPix;
    float compressw, compressh;
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        fbo.begin();ofClear(0);fbo.end();
        compressh = compressw = 0.05f;
    }
    
    void stateEnter()
    {
        show();
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
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4] = ofNoise(i*0.001+t*0.001, j*0.001+t*0.001) * 50.f*alpha;
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+1] = ofNoise(i*0.001-t*0.001, j*0.001+t*0.002) * 127.f*alpha;
                        colorPixels->getPixels()[(i+j*SVG_WIDTH)*4+2] = ofNoise(i*0.001-t*0.002, j*0.001-t*0.001) * 255.f * alpha;
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
        fbo.begin();
        ofClear(0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        tex->draw(0, 0);
        sharedData->drawStars(alpha);
        ofDisableBlendMode();
        fbo.end();
        fbo.readToPixels(*colorPixels);
        tex->loadData(*colorPixels);
    }
    
    void draw()
    {
        BaseState::draw();
        sharedData->location.drawLed();
        
    }
    
    string getName()
    {
        return "NoiseState";
    }
};


#endif
