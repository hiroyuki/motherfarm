//
//  BellState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/15/12.
//
//

#ifndef motherfarmLED_BellState_h
#define motherfarmLED_BellState_h
#include "BaseState.h"
#define RECT_NUM_X 60.f
#define RECT_NUM_Y 5.f
class ColorRect
{
public:
    float x, y;
    
    void update()
    {
    }
    
    void draw()
    {
        ofSetColor(255, 255, 0);
        ofRect(x, y, 5, 5);
    }
};
class BellState : public BaseState
{
public:
    ofTexture *tex;
    ofFbo fbo;
    ofPixels * colorPixels;
    int longestLen;
    vector<ColorRect> rects;
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        fbo.begin();ofClear(0);fbo.end();
        assignRect();
    }
    
    void stateEnter()
    {
        assignRect();
    }
    
    void assignRect()
    {
        rects.clear();
        for( float i = 0; i <= longestLen / RECT_NUM_X; i++)
//        float i = longestLen / 2.f / RECT_NUM_X;
        {
            for( int j = 0; j <= longestLen / RECT_NUM_Y; j++)
            {
                rects.push_back(ColorRect());
                rects.back().x = i * RECT_NUM_X;
                rects.back().y = j * RECT_NUM_Y;
            }
        }
    }
    
    void update()
    {
        BaseState::update();
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        fbo.begin();
        ofSetColor(240, 240, 240, 240);
        fbo.draw(0, 0);
        ofSetHexColor(0xffffff);
        ofPushMatrix();
        ofTranslate( (SVG_WIDTH-longestLen)>>1,(SVG_HEIGHT-longestLen)>>1);
        ofTranslate(longestLen/2, longestLen/2);
        ofRotate(ofGetElapsedTimeMillis()/ 10.f, 0, 0, 1);
        for( int i = 0; i < rects.size(); i++ )
        {
            ofPushMatrix();
            ofTranslate(-longestLen / 2, -longestLen / 2);
            rects[i].update();
            rects[i].draw();
            ofPopMatrix();
        }
        ofPopMatrix();
        fbo.end();
        fbo.readToPixels(*colorPixels);
        tex->loadData(colorPixels->getPixels(), SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
    }
    
    void draw()
    {
        BaseState::draw();
        sharedData->location.drawLed();
    }
    
    string getName()
    {
        return "BellState";
    }
};


#endif
