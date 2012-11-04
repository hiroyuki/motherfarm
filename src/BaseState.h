//
//  BaseState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/4/12.
//
//

#ifndef motherfarmLED_BaseState_h
#define motherfarmLED_BaseState_h
class BaseState : public Apex::ofxState<SharedData>
{
public:
    float alpha;
    bool isShowing, isHiding;
    string nextState;
    SharedData *sharedData;
    float showLerp, hideLerp;
    virtual void setup()
    {
        sharedData = &getSharedData();
        showLerp = hideLerp = 0.05;
        alpha = 1;
    }
    
    virtual void stateEnter()
    {
        show();
    }
    
    virtual void stateExit()
    {
        
    }
    
    virtual void update()
    {
        if ( isShowing )
        {
            alpha = ofLerp(alpha, 1., showLerp);
            if ( alpha > 0.99)
            {
                alpha = 1;
                isShowing = false;
            }
        }
        if ( isHiding )
        {
            alpha = ofLerp( alpha, 0.f, hideLerp);
            if ( alpha < 0.01)
            {
                alpha = 0;
                isHiding = false;
            }
        }
        sharedData->location.update();
    }
    
    virtual void draw()
    {
        //        ofBackgroundGradient(ofColor(255/5,255/5,255/5),ofColor(0));
        ofBackground(0, 0, 0);
        ofSetColor(255, 255, 255, alpha*255);
        if ( sharedData->showParse )
        {
            ofEnableLighting();
            sharedData->location.debugDraw();
        }
    }
    
    virtual void show()
    {
        if( !isHiding ) isShowing = true;
    }
    
    virtual void hide(string nextState)
    {
        if ( !isShowing) isHiding = true;
    }
    
    virtual void eventListener()
    {
        
    }
    
    virtual string getName()
    {
        return "BaseState";
    }
    
    virtual void setColor( int r, int g, int b)
    {
        ofSetColor(r, g, b, alpha* 255);
    }
};
#endif
