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
#define LERP 0.01
#define CENTER_LERP 0.03
class CircleToCenter : public CircleSeed
{
public:
    ofPoint center;
    float lastPosSeed, curPosSeed;
    float angleRad;
    float centerDegree;
    float longestLen;
    bool direction;
    int no;
    int delay, changeMs;
    float speed;
    float pastScale, curScale;
    
    
    CircleToCenter( int _no = 0, bool toCenter = false) : CircleSeed()
    {
        direction = toCenter;
        no = _no;
        if ( direction )
        {
            speed = CENTER_LERP;
        }
        else
        {
            speed = CENTER_LERP / 5.f;
        }
    }
    
    void setSpeed( float sp )
    {
        speed = sp;
    }
    
    virtual void init(int width = 0, int height = 0, int _longestLen = 0)
    {
        longestLen = (_longestLen != 0) ? _longestLen : longestLen;
        if ( width != 0 && height != 0)
        {
            CircleSeed::init(width, height);
            delay = ofRandom(1000) + no * 30;
            changeMs = ofGetElapsedTimeMillis();
        }
        else //2回め以降
        {
            delay = 0;
        }
        color.r = ofRandom(255);
        color.g = ofRandom(255);
        color.b = ofRandom(255);
        
        center = ofPoint( areaW / 2.f, areaH / 2.f - 30.f, 0.f );
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
        lastPosSeed = 0;
        radius = ofRandom(5, 10);
        status = STATUS_NEW;
    } 
    
    void update()
    {
        lastPosSeed = longestLen * alpha;
        pastScale = radius*ofMap(centerDegree, 0, 0.3, 0, 1, true);
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
                centerDegree = ofLerp(centerDegree, direction ? 0 : 1, speed);
                alpha = ofLerp(centerDegree, direction ? 0 : 1, speed);
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
        curPosSeed = longestLen * alpha;
        curScale = radius*ofMap(centerDegree, 0, 0.3, 0, 1, true);
    }

    virtual void draw()
    {
        if ( status == STATUS_SHOW)
        {
            float lastPosx = center.x + cos(angleRad) * lastPosSeed;
            float lastPosy = center.y + sin(angleRad) * lastPosSeed;
            pos.x = center.x + cos(angleRad) * curPosSeed;
            pos.y = center.y + sin(angleRad) * curPosSeed;
            glLineWidth(5);
            glBegin(GL_LINE_STRIP);
            glVertex2d(pos.x, pos.y);
            glVertex2d(lastPosx, lastPosy);
            glEnd();
            glLineWidth(1);
        }
    }
    virtual void drawColor(float al)
    {
        ofSetColor(color.r, color.g, color.b, 255.f * al);
        if ( alpha != 0)
        {
            float lastPosx = center.x + cos(angleRad) * lastPosSeed;
            float lastPosy = center.y + sin(angleRad) * lastPosSeed;
            pos.x = center.x + cos(angleRad) * curPosSeed;
            pos.y = center.y + sin(angleRad) * curPosSeed;
            glLineWidth(5);
            glBegin(GL_LINE_STRIP);
            glVertex2d(pos.x, pos.y);
            glVertex2d(lastPosx, lastPosy);
            glEnd();
            glLineWidth(1);
        }
    }
};
#endif
