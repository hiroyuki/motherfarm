//
//  Circle.h
//  motherfarmLED
//
//  Created by motherfarm on 11/19/12.
//
//

#ifndef motherfarmLED_Circle_h
#define motherfarmLED_Circle_h
enum CIRCLE_STATUS
{
    CIRCLE_READY, CIRCLE_SHOW, CIRCLE_HIDE
};

class StaticCircle
{
public:
    ofPoint pos;
    ofColor color;
    float alpha;
    int radius;
    CIRCLE_STATUS status;
    void setup()
    {
        status = CIRCLE_SHOW;
        color = ofColor(255,255,255);
        radius = ofRandom( 5, 10 );
        pos = ofPoint(ofRandom(SVG_WIDTH), ofRandom(SVG_HEIGHT));
    }
    
    void update()
    {
        if (status == CIRCLE_SHOW)
        {
            alpha = ofLerp( alpha, 1, 0.1f);
            if ( alpha > 0.95f)
            {
                status = CIRCLE_HIDE;
            }
        }
        if ( status == CIRCLE_HIDE)
        {
            alpha = ofLerp( alpha, 0, 0.1f);
            if ( alpha < 0.05 )
            {
                alpha = 0;
                setup();
            }
        }
    }
    
    void draw()
    {
        ofSetColor( color, alpha * 255.f );
        ofCircle( pos, radius);
    }
};


#endif
