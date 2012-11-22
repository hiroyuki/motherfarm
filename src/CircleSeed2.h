//
//  CircleSeed2.h
//  motherfarmLED
//
//  Created by motherfarm on 11/20/12.
//
//

#ifndef motherfarmLED_CircleSeed2_h
#define motherfarmLED_CircleSeed2_h
#define LERP 0.5
#define SCALE_LERP 0.4
#define KEEP_MS 0
#define STATUS_NEW 0
#define STATUS_SHOW 1
#define STATUS_KEEP 2
#define STATUS_HIDE 3
class CircleSeed2
{
public:
    ofColor color;
    int radius;
    ofPoint pos;
    float alpha;
    float scale;
    int status;
    int areaW, areaH;
    
    CircleSeed2():color(0xff,0xff, 0xff), radius(40), alpha(0), status(0), scale(0)
    {
    }
    
    virtual void init( int width = 0, int height = 0)
    {
        if ( height != 0) areaH = height;
        if ( width != 0) areaW = width;
        pos = ofPoint(ofRandom(areaW), ofRandom(areaH));
        radius = ofRandom(5, 20);
        status = STATUS_NEW;
        color.r = ofRandom(255);
        color.g = ofRandom(255);
        color.b = ofRandom(255);
    }
    
    virtual void update(bool gotBang, float _alpha)
    {
        if ( status > STATUS_NEW){
            scale = ofLerp(scale, 1, SCALE_LERP);
        }
        switch (status) {
            case STATUS_NEW:
                if ( gotBang )
                {
                    status = STATUS_SHOW;
                }
                alpha = 0;
                scale = 0;
                break;
            case STATUS_SHOW:
                alpha = ofLerp(alpha, 1, LERP);
                if( alpha > 0.99)
                {
                    status = STATUS_HIDE;
                    alpha = 1;
                }
                break;
            case STATUS_HIDE:
                alpha = ofLerp( alpha, 0, LERP);
                if ( alpha < 0.01)
                {
                    alpha = 0;
                    init();
                }
                break;
            default:
                break;
        }
        
    }
    
    virtual void draw(float al = 1.f)
    {
        ofSetColor(color, al*alpha*255);
        ofCircle(pos.x, pos.y, radius*scale);
    }

};

#endif
