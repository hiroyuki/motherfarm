//
//  CircleSeed.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_CircleSeed_h
#define motherfarmLED_CircleSeed_h
#define LERP 0.3
#define SCALE_LERP 0.15
#define KEEP_MS 0
#define STATUS_NEW 0
#define STATUS_SHOW 1
#define STATUS_KEEP 2
#define STATUS_HIDE 3

class CircleSeed
{
public:
    ofColor color;
    int radius;
    ofPoint pos;
    float alpha;
    float scale;
    int status;
    int changeMs;
    int areaW, areaH;
    int delay;
    
    CircleSeed():color(0xff,0xff, 0xff), radius(40), alpha(0), status(0), scale(0)
    {
        delay = ofRandom(1000);
    }
    
    virtual void init( int width = 0, int height = 0)
    {
        if ( height != 0) areaH = height;
        if ( width != 0) areaW = width;
        pos = ofPoint(ofRandom(areaW), ofRandom(areaH));
        radius = ofRandom(30, 140);
        status = STATUS_NEW;
        changeMs = ofGetElapsedTimeMillis();
        color.r = ofRandom(255);
        color.g = ofRandom(255);
        color.b = ofRandom(255);
    }
    
    virtual void update()
    {
        if ( status > STATUS_NEW){
            scale = ofLerp(scale, 1, SCALE_LERP);
        }
        switch (status) {
            case STATUS_NEW:
                if ( ofGetElapsedTimeMillis() - changeMs > delay)
                {
                    status = STATUS_SHOW;
                    cout << status << endl;
                    changeMs = ofGetElapsedTimeMillis();    
                }
                alpha = 0;
                scale = 0;
                break;
            case STATUS_SHOW:
                alpha = ofLerp(alpha, 1, LERP);
                if( alpha > 0.99)
                {
                    status = STATUS_KEEP;
                    cout << status << endl;
                    changeMs = ofGetElapsedTimeMillis();
                    alpha = 1;
                }
                break;
            case STATUS_KEEP:
                if ( ofGetElapsedTimeMillis() - changeMs > KEEP_MS)
                {
                    changeMs = ofGetElapsedTimeMillis();
                    status = STATUS_HIDE;
                    cout << status << endl;
                }
                break;
            case STATUS_HIDE:
                alpha = ofLerp( alpha, 0, LERP);
                if ( alpha < 0.01)
                {
                    alpha = 0;
                    init();
                    changeMs = ofGetElapsedTimeMillis();
                }
                break;
            default:
                break;
        }
    
    }
    
    virtual void draw()
    {
        ofSetColor(color, alpha*255);
        ofCircle(pos.x, pos.y, radius*scale);
    }
    
};

#endif
