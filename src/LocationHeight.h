//
//  LocationHeight.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 10/25/12.
//
//

#ifndef motherfarmLED_LocationHeight_h
#define motherfarmLED_LocationHeight_h
#include "ofxAssimpModelLoader.h"

class LocationHeight
{
public:
    ofxAssimpModelLoader model;
    void setup(string filename);
    void debugDraw();
    void setupScale();
    float getHeight(ofPoint point);
};


#endif
