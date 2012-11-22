//
//  BaseState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/4/12.
//
//

#ifndef motherfarmLED_BaseState_h
#define motherfarmLED_BaseState_h
#include "ofxSimpleGuiToo.h"
class BaseState
{
protected:
    bool isActive = false;
    bool modeChanged = false;
    bool pastActive = false;
public:
    float alpha;
    bool isShowing, isHiding;
    SharedData *sharedData;
    float showLerp, hideLerp;
    int showMs;
    bool doUpdate;
    ofFbo* fbo;
    
    ofTexture *tex;
    ofPixels * colorPixels;
    BaseState(SharedData* sharedData):isActive(false), modeChanged(false)
    {
        this->sharedData = sharedData;
    }
    
    BaseState() : isActive(false), modeChanged(false){}
    
    virtual void setSharedData( SharedData *sharedData)
    {
        this->sharedData = sharedData;
    
    }
    
    virtual void setup()
    {
        showLerp = hideLerp = 0.05;
        alpha = 1;
        fbo = new ofFbo();
        fbo->allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        colorPixels = new ofPixels();
        colorPixels->allocate(SVG_WIDTH, SVG_HEIGHT, 4);
        fbo->begin();ofClear(0);fbo->end();
        tex = sharedData->tex;
    }
    
    virtual void stateEnter()
    {
        showMs = ofGetElapsedTimeMillis();
        fbo->begin();ofClear(0);fbo->end();
    }
    
    virtual void stateExit()
    {
        doUpdate = false;
    }
    
    virtual void changeActive( bool bActive )
    {
        if ( isActive != bActive)
        {
            isActive = bActive;
            modeChanged = true;
        }
    }
    
    virtual bool getIsActive()
    {
        return isActive;
    }
    
    virtual bool bdoUpdate()
    {
        return doUpdate;
    }
    
    virtual void update()
    {
        if (isActive) doUpdate = true;
        glDisable(GL_DEPTH_TEST);
        if ( pastActive != isActive)
        {
            modeChanged = true;
        }
        if ( modeChanged )
        {
            if ( isActive ) show();
            else hide();
        }
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
                stateExit();
                isHiding = false;
            }
        }
        modeChanged = false;
        pastActive = isActive;
    }
    
    virtual ofFbo* getFbo()
    {
        return fbo;
    }
    
    virtual void draw()
    {
//        cout << alpha << endl;
        //        ofBackgroundGradient(ofColor(255/5,255/5,255/5),ofColor(0));
        ofSetColor(255, 255, 255, alpha*255);
        fbo->draw(0,0);
    }
    
    virtual void show()
    {
        cout << "show " << getName()<< endl;
//        if( !isHiding )
        {
            isShowing = true;
            isHiding = false;
            alpha = 0;
            stateEnter();
        }
    }
    
    virtual void hide()
    {
        {
            isShowing = false;
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
