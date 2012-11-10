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
class CircleToCenter : public CircleSeed
{
public:
    ofPoint center;
    float angleRad;
    float longestLen;
    bool direction;
    CircleToCenter( bool toCenter = false) : CircleSeed()
    {
        direction = toCenter;
    }
    
    void init(int width, int height, int _longestLen)
    {
        longestLen = _longestLen;
        CircleSeed::init(width, height);
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
        delay = ofRandom(6000);
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
                alpha = direction ? 1 : 0;
                break;
            case STATUS_SHOW:
                alpha = ofLerp(alpha, direction ? 0 : 1, LERP);
                if( (alpha < 0.01 && direction ) || (alpha > 0.99 && !direction ))
                {
                    status = STATUS_HIDE;
                    changeMs = ofGetElapsedTimeMillis();
                    alpha = direction ? 1 : 0;
                }
                break;
            case STATUS_HIDE:
                break;
            default:
                break;
        }
        pos.x = center.x + cos( angleRad) * longestLen*alpha;
        pos.y = center.y + sin( angleRad) * longestLen*alpha;
    }
    
    virtual void draw()
    {
        ofSetColor(color);
//        ofSetColor(0xffffff);
        ofCircle(pos.x, pos.y, radius*ofMap(alpha, 0, 0.3, 0, 1, true));
    }
};
#endif
