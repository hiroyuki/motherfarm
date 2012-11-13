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
//#define RISE
#ifdef RISE
#include "RiseLocationLine.h"
#else
#include "LocationLine.h"
#endif
#include "ofxGrabCam.h"
#include "LocationHeight.h"
#include "ofxOsc.h"
#ifdef RISE
#define TEX_OFFSET_X 0
#define TEX_OFFSET_Y 0
#else
#define TEX_OFFSET_X -200
#define TEX_OFFSET_Y -196
#endif
class Location
{
public:
    void setup(ofTexture * colorPix, ofPixels * pix);
    void update();
    void debugDraw();
    void drawLed();
    void exportXML();
    void drawDebugLine();
    void draw2dLine();
    void loadSVG(string filename);
    void placeLedOnWorld();
    void changeLightPos(int x, int y, int z);
    void setLight(float ambient_r, float ambient_g, float ambient_b,
                  float diffuse_r, float diffuse_g, float diffuse_b);
    void setCameraPos( float x, float y, float z);
    
    
    ofTexture * colorTexture;
    ofPixels * colorPix;
//    void exportLocation();
    ofFbo depthBufferFbo;
    LocationLine* getLine(int group, int nordIdInGroup, int dmxIndex);
#ifdef RISE
    vector<RiseLocationLine> lines;
#else
    vector<LocationLine> lines;
#endif
    ofxSVGTiny svg;
    LocationHeight mapHeight;
    ofLight light;
    ofxGrabCam cam;
};


#endif
