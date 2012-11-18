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
#define LERP 0.007
class ScaleCircle : public CircleSeed
{
public:
    int pastRadius;
    ofFbo fbo;
    ofImage img;
    
    ScaleCircle(int no) : CircleSeed()
    {
        delay = ofRandom(2000) + no * 2000;
    }
    
    void init( int width, int height)
    {
        
        CircleSeed::init(width, height);
        pos = ofPoint(width/2 + ofRandom(-1, 1) * width * 0.15, height/2 + ofRandom(-1, 1)*height * 0.15);
        radius = 0;
        pastRadius = 0;
        if ( !fbo.isAllocated()) fbo.allocate(width, height, GL_RGBA, 0);
        if ( !img.isAllocated()) img.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
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
                if( alpha > 0.7)
                {
                    status = STATUS_HIDE;
                    changeMs = ofGetElapsedTimeMillis();
                    alpha = 0;
                    delay=ofRandom(1000);
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
        if ( radius > pastRadius ) {
            ofSetLineWidth(3);
            ofSetColor(color, ofMap(alpha, 0, 0.01, 0, 1, true)*255);
            ofCircle(pos.x, pos.y, radius);
            ofSetHexColor(0);
            ofCircle(pos.x, pos.y, pastRadius);
            pastRadius = radius;
        }
        else
        {
            pastRadius = radius - 1;
        }
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
