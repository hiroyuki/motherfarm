//
//  SingleColorWave.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/5/12.
//
//

#ifndef motherfarmLED_SingleColorWave_h
#define motherfarmLED_SingleColorWave_h
#include "BaseState.h"
#include "WaveSeed.h"
#define MAX_LINE 20

class SingleColorWave : public BaseState
{
public:
    ofTexture *tex;
    ofPixels * colorPixels;
    int longestLen;
    vector<WaveSeed> waves;
    
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        waves.push_back(WaveSeed());
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
    }
    
    void update()
    {
        BaseState::update();
        //        for (int i = 0; i < SVG_WIDTH; i++){
        //            for (int j = 0; j < SVG_HEIGHT; j++){
        //                float val = ofNoise((i/SVG_WIDTH+j/SVG_HEIGHT)*5-ofGetFrameNum()*0.03);
        //                colorPixels->getPixels()[(j*(int)SVG_WIDTH+i)*3 + 0] = (1-val) * 255;	// r
        //                colorPixels->getPixels()[(j*(int)SVG_WIDTH+i)*3 + 1] = (1-val) * 255;	// g
        //                colorPixels->getPixels()[(j*(int)SVG_WIDTH+i)*3 + 2] = (val) * 255; // b
        //            }
        //        }
        for( int i = 0; i < waves.size(); i++)
        {
            if ( waves[ i ].update() )
            {
                if ( waves.size() < MAX_LINE )
                    waves.push_back(WaveSeed());
            }
            if ( waves[ i ].posY > longestLen)
            {
                waves[ i ].init();
            }
        }
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        
        fbo.begin();
        ofSetColor(220, 220, 220, 254);
        fbo.draw(0, 0);
        ofSetColor(255, 255, 255);
        //        glLineWidth(3);
        glPointSize(2);
        
        ofPushMatrix();
        ofTranslate(longestLen/2, longestLen/2);
        ofRotate(sharedData->angle, 0, 0, 1);
        for( int j = 0; j < waves.size(); j++)
        {
            ofSetColor( waves[ j ].color );
            glBegin(GL_POINTS);
            for( int k = 0; k < longestLen; k++)
            {
                glVertex2d(k-longestLen/2, (int)waves[j].getY(k, fbo.getHeight()*0.2)-longestLen/2);
            }
            glEnd();
        }
        ofPopMatrix();
        
        
        fbo.end();
        fbo.readToPixels(*colorPixels);
        tex->loadData(colorPixels->getPixels(), SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
    }
    
    void draw()
    {
        //        cout << endl;
        
        BaseState::draw();
        sharedData->location.drawLed();
        
        //        ofDisableLighting();
        //        glLineWidth(1);
        //        ofNoFill();
        //        ofRect((longestLen - fbo.getWidth())/2,(longestLen - fbo.getHeight())/2, fbo.getWidth(), fbo.getHeight());
        //        ofFill();
        //        ofPushMatrix();
        //        ofSetHexColor(0xffffff);
        //        ofTranslate((longestLen - fbo.getWidth())/2,(longestLen - fbo.getHeight())/2);
        //        fbo.draw(0,0);
        //        ofPopMatrix();
        
    }
    
    string getName()
    {
        return "SingleColorWave";
    }
};
#endif
