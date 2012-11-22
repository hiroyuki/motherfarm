//
//  DanganCIrcle.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_DanganCIrcle_h
#define motherfarmLED_DanganCIrcle_h
#define MAX_CIRCLE 60
class DanganCircle : public CircleToCenter
{
public:
    DanganCircle( int _no = 0, bool toCenter = false) : CircleToCenter(_no, toCenter)
    {
    }
    
    void init(int width = 0, int height = 0, int _longestLen = 0)
    {
        longestLen = (_longestLen != 0) ? _longestLen : longestLen;
        if ( width != 0 && height != 0)
        {
            CircleSeed::init(width, height);
            delay = no * 150;
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
        angleRad = no * 0.1f;
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
};

#endif
