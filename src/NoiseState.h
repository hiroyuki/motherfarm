//
//  NoiseState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/15/12.
//
//

#ifndef motherfarmLED_NoiseState_h
#define motherfarmLED_NoiseState_h
class NoiseState : public BaseState
{
public:
    ofTexture *tex;
    ofFbo fbo;
    ofPixels * colorPixels;
    int longestLen;
    
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        fbo.begin();ofClear(0);fbo.end();
    }
    
    void update()
    {
        
        
    }
    
    void draw()
    {
        
    }
    
    string getName()
    {
        return "NoiseState";
    }
};


#endif
