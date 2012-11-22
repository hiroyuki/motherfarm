//
//  WaveSeed.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_WaveSeed_h
#define motherfarmLED_WaveSeed_h

class WaveSeed
{
private:
    int nextInterval;
public:
    float posY;
    float seed;
    bool addedNext;
    ofColor color;
    int startMs;
    float angle;
    bool isAdded = false;
    
    WaveSeed(int nextInterval, float angle):posY(0), addedNext(false), color(0xffffff)
    {
        seed = ofRandom(1);
        this->nextInterval = nextInterval*3;
        this->angle = angle;
        startMs = ofGetElapsedTimeMillis();
    }
    
    int getInterval()
    {
        return nextInterval - ( ofGetElapsedTimeMillis() - startMs);
    }
    
    float getY( int i, int height )
    {
        return (int)(ofNoise(i*0.01, (i-ofGetFrameNum()*seed)*0.01)*height*ofNoise(seed+ofGetFrameNum()*seed*0.02) + posY);
    }
    
    bool update(float speed)
    {
        posY += speed;
        if ( nextInterval < posY && !addedNext)
        {
            addedNext = true;
            return true;
        }
        return false;
    }
    
    void init(int nextInterval, float angle)
    {
        this->angle = angle;
        this->nextInterval = nextInterval*3;
        seed = ofRandom(1);
        posY = 0;
        addedNext = false;
        startMs = ofGetElapsedTimeMillis();
        isAdded= false;
    }
};

#endif
