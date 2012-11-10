//
//  CircleStar.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_CircleStar_h
#define motherfarmLED_CircleStar_h
#include "CircleSeed.h"
class CircleStar : public CircleSeed
{
public:
    void init(int width, int height)
    {
        CircleSeed::init(width, height);
        pos = ofPoint(ofRandom(width), 0);
        radius = ofRandom(10);
        delay = ofRandom(4000);
    }
    
    void update()
    {
        CircleSeed::update();
        pos.y += 5;
    }
    
    virtual void draw(int offsetx, int offsety)
    {
        ofSetColor(color, alpha*255);
        ofCircle(pos.x-offsetx, pos.y-offsety, radius);
    }
};


#endif
