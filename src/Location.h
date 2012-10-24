//
//  Location.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 10/23/12.
//
//

#ifndef motherfarmLED_Location_h
#define motherfarmLED_Location_h
#include "ofxSVGTiny.h"
#include "LocationLine.h"
class Location
{
public:
    void setup();
    void update();
    void debugDraw();
    void exportXML();
    LocationLine* getLine(int group, int nordIdInGroup, int dmxIndex);
    vector<LocationLine> lines;
    ofxSVGTiny svg;
};


#endif
