//
//  CircleToCenter.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_CircleToCenter_h
#define motherfarmLED_CircleToCenter_h
#include "CircleSeed.h"
#define LERP 0.03
#define CENTER_LERP 0.03
class CircleToCenter : public CircleSeed
{
public:
    ofPoint center;
    float angleRad;
    float centerDegree;
    float longestLen;
    bool direction;
    int no;
    CircleToCenter( int _no = 0, bool toCenter = false) : CircleSeed()
    {
        direction = toCenter;
        no = _no;
    }
    
    void init(int width = 0, int height = 0, int _longestLen = 0)
    {
        longestLen = (_longestLen != 0) ? _longestLen : longestLen;
        if ( width != 0 && height != 0)
        {
            CircleSeed::init(width, height);
            delay = ofRandom(1000) + no * 500;
        }
        else //2回め以降
        {
            delay = 0;
        }
        color.r = 255;
        color.g = 255;
        color.b = 0;
        
        center = ofPoint( areaW / 2.f, areaH / 2.f, 0.f );
        angleRad = ofRandom( TWO_PI );
        if ( direction )
        {
            pos.x = center.x + cos( angleRad ) * _longestLen;
            pos.y = center.y + sin( angleRad ) * _longestLen;
        }
        else
        {
            pos.x = center.x;
            pos.y = center.y;
        }
        
        radius = ofRandom(5, 20);
        status = STATUS_NEW;
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
                centerDegree = direction ? 1 : 0;
                alpha = direction ? 1 : 0;
                break;
            case STATUS_SHOW:
                centerDegree = ofLerp(centerDegree, direction ? 0 : 1, CENTER_LERP);
                alpha = ofLerp(centerDegree, direction ? 0 : 1, LERP);
                if( (alpha < 0.01 && direction ) || (alpha > 0.99 && !direction ))
                {
                    status = STATUS_HIDE;
                    changeMs = ofGetElapsedTimeMillis();
                    centerDegree = alpha = direction ? 0 : 1;
                }
                break;
            case STATUS_HIDE:
                init();
                break;
            default:
                break;
        }
        pos.x = center.x + cos( angleRad) * longestLen*alpha;
        pos.y = center.y + sin( angleRad) * longestLen*alpha;
    }
    
    virtual void draw()
    {
        ofSetColor(color.r, color.g, color.b, ofMap( alpha, 0, 0.15, 0, 1, true ) * 255);
        ofCircle(pos.x, pos.y, radius*ofMap(centerDegree, 0, 0.3, 0, 1, true));
    }
};
#endif
