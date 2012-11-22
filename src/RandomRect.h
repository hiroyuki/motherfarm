//
//  RandomRect.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_RandomRect_h
#define motherfarmLED_RandomRect_h
enum RECT_MODE
{
   ONE_MINUS, NORMAL
};

enum STATUS{
    READY, SHOWING, HIDING
};

class RandomRect
{
public:
    ofPoint pos;
    int size;
    ofColor color;
    float colorDepth;
    STATUS status;
    int mode;
    int delay;
    int startms;
    float lerp;
    
    void setup(int x, int y, int size)
    {
        mode = NORMAL;
        if ( mode == ONE_MINUS )
        {
            lerp = 0.1;
        }
        else
        {
            lerp = 0.02;
        }
        pos.x = x;
        pos.y = y;
        colorDepth = 0;
        delay = 250 * (int)ofRandom(20);
        this->size = size;
        color = ofColor::fromHsb(ofRandom(255), 127, 127);
    }
    
    void start()
    {
        startms = ofGetElapsedTimeMillis();
        status = READY;
    }
    
    void update()
    {
//        if ( mode == NORMAL)
        {
            if ( status == READY)
            {
                if (ofGetElapsedTimeMillis() - startms > delay){
                    status = SHOWING;
                }
            }
            else if ( status == SHOWING)
            {
                colorDepth = ofLerp(colorDepth, 1.f, lerp);
                if ( colorDepth > 0.99)
                {
                    status = HIDING;
                }
            }
            else if ( status == HIDING )
            {
                colorDepth = ofLerp(colorDepth, 0.1f, lerp);
                if ( colorDepth < 0.12)
                {
                    status = READY;
                    color = ofColor::fromHsb(ofRandom(255), 127, 127);
                }
            }
            
        }
    }
    
    void draw(float alpha)
    {
        if ( mode == ONE_MINUS )
        {
            ofSetColor(ofColor::white - color * (colorDepth));
        }
        else
        {
            ofSetColor(color * colorDepth * alpha);
        }
        ofRect(pos.x, pos.y, size, size);
    }
};


#endif
