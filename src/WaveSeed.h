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
public:
    float posY;
    float speed;
    float seed;
    bool addedNext;
    int nextInterval;
    ofColor color;
    
    WaveSeed() : posY(0), speed(3), addedNext(false), color(0xffffff){
        seed = 0;//ofRandom(0.1, 0.2);
        nextInterval = ofRandom(300, 500);
        
    };
    float getY( int i, int height )
    {
        return (int)(ofNoise(i*0.01, (i-ofGetFrameNum()*seed)*0.01)*height*ofNoise(seed+ofGetFrameNum()*seed*0.02) + posY);
    }
    
    bool update()
    {
        posY += speed;
        cout << nextInterval << " " << posY << endl;
        if ( nextInterval < posY && !addedNext)
        {
            addedNext = true;
            return true;
        }
        return false;
    }
    
    void init()
    {
        seed = ofRandom(1);
        posY = 0;
        addedNext = false;
    }
};

#endif
