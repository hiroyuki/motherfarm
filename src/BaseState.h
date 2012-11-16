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
            if ( alpha < 0.1)
            {
                alpha = 0;
                isHiding = false;
                sharedData->dt.eventName = "showState";
                sharedData->dt.nextState = nextState;
                ofNotifyEvent(sharedData->event.farmEvent, sharedData->dt, this);
            }
        }
        sharedData->location.update();
    }
    
    virtual void draw()
    {
//        cout << alpha << endl;
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
        cout << "show " << getName()<< endl;
//        if( !isHiding )
        {
            isShowing = true;
            isHiding = false;
            alpha = 0;
        }
    }
    
    virtual void hide(string _nextState)
    {
        cout << "hide to " << _nextState <<getName() << endl;
        if ( _nextState != getName())
        {
            isShowing = false;
            nextState = _nextState;
            isHiding = true;
        }
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
