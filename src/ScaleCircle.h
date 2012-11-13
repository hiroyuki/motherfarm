//
//  ScaleCircle.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_ScaleCircle_h
#define motherfarmLED_ScaleCircle_h
#include "CircleSeed.h"
#define LERP 0.005
class ScaleCircle : public CircleSeed
{
public:
    int pastRadius;
    ofFbo fbo;
    ofImage img;
    
    ScaleCircle() : CircleSeed()
    {
        delay = ofRandom(10000);
    }
    
    void init( int width, int height)
    {
        
        CircleSeed::init(width, height);
        pos = ofPoint(width/2, height/2);
        radius = 0;
        pastRadius = 0;
        fbo.allocate(width, height);
        img.allocate(width, height, OF_IMAGE_COLOR);
    }
    
    void update()
    {
        switch (status) {
            case STATUS_NEW:
                if ( ofGetElapsedTimeMillis() - changeMs > delay)
                {
                    status = STATUS_SHOW;
                    changeMs = ofGetElapsedTimeMillis();
                }
                alpha = 0;
                break;
            case STATUS_SHOW:
                alpha = ofLerp(alpha, 1, LERP);
                if( alpha > 0.99)
                {
                    status = STATUS_HIDE;
                    changeMs = ofGetElapsedTimeMillis();
                    alpha = 1;
                    delay=0;
                }
                break;
            default:
                break;
        }
    }
    
    void updateImage( int longlen)
    {
        
        fbo.begin();
        ofClear(0);
        radius = alpha * longlen;
        ofSetLineWidth(3);
        ofSetColor(color, ofMap(alpha, 0, 0.1, 0, 1, true)*255);
        ofCircle(pos.x, pos.y, radius);
        ofSetHexColor(0);
        ofCircle(pos.x, pos.y, pastRadius);
        pastRadius = radius;
        fbo.end();
        fbo.readToPixels(img.getPixelsRef());
        img.update();
    }
    
    void draw(int longlen)
    {
//        fbo.draw(0,0);
        img.draw(0, 0);
    }
};

#endif
