//
//  GalaxyOfStarState.h
//  motherfarmLED
//
//  Created by Akira on 11/12/12.
//
//

#ifndef motherfarmLED_GalaxyOfStarState_h
#define motherfarmLED_GalaxyOfStarState_h

#include "BaseState.h"
#include "ofxTweenLite.h"
#include "ofxFadable.h"

const int numStars = 300;
const float initialPositioningDur = 0.1;
const float shootingDur = 4.0;
const int numMaxTrail = 50;

class Star : public ofxFadableBase
{
public:
    
    void setup(ofColor _col)
    {
        initialdestPos = ofPoint(ofRandom(-SVG_WIDTH/2, SVG_WIDTH+SVG_WIDTH/2),
                                 ofRandom(-SVG_HEIGHT/2, SVG_HEIGHT+SVG_HEIGHT/2),
                                 ofRandom(-2500, -5500));
        initialPos = initialdestPos;
        initialPos.z += 1000;
        curPos = initialPos;
        shootDestPos = initialPos;
        shootDestPos.z += 6000;
        shootBasePos = shootDestPos;
        col = _col;
        rad = ofRandom(40, 20);
        
        initializeTime = 0.0;
        bInitialized = false;
        bMainmovement = false;
        shoothingTime = 0.0;
        bShootingStar = false;
        bShoothing = false;
        bFinishShooting = false;
        
        init();
        setAlpha(0);
        setFadeSeconds(2.0);
        fadeIn();
    }
    
    void update()
    {
        updateFade();
        
        if (!bInitialized && !bMainmovement)
        {
            initializeTime = ofGetElapsedTimef();
            bInitialized = true;
        }
        else if (bInitialized && !bMainmovement)
        {
            float cur = ofGetElapsedTimef();
            float diff = cur - initializeTime;
            float mappedProgress = ofMap(diff, 0.0, initialPositioningDur, 0.0, 1.0, true);
            float factor = ofxTweenLite::tween(0.0, 1.0, mappedProgress, OF_EASE_EXPO_IN);
            curPos = initialPos.getInterpolated(initialdestPos, factor);
            if (mappedProgress >= 1.0)
                bMainmovement = true;
        }
        else if (bMainmovement)
        {
            if (bShootingStar)
            {
                if (!bShoothing)
                {
                    shoothingTime = ofGetElapsedTimef();
                    shootBasePos = curPos;
                    bShoothing = true;
                }
                else
                {
                    float cur = ofGetElapsedTimef();
                    float diff = cur - shoothingTime;
                    float mappedProgress = ofMap(diff, 0.0, shootingDur, 0.0, 1.0, true);
                    float factor = ofxTweenLite::tween(0.0, 1.0, mappedProgress, OF_EASE_EXPO_INOUT);
                    curPos = shootBasePos.getInterpolated(shootDestPos, factor);
                    
                    if (trails.size() > 0)
                    {
                        ofPoint prev = trails.at(trails.size()-1);
                        int numInterp = 10;
                        ofPolyline trailLine;
                        trailLine.addVertex(prev);
                        trailLine.addVertex(curPos);
                        trailLine = trailLine.getResampledByCount(numInterp);
                        vector<ofPoint> line = trailLine.getVertices();
                        for (int i = 0; i < line.size(); i++)
                        {
                            if (trails.size() > numMaxTrail)
                                trails.pop_front();
                            trails.push_back(line.at(i));
                        }
                    }
                    else
                    {
                        trails.push_back(curPos);
                    }
                    
                    if (mappedProgress >= 1.0)
                    {
                        bShoothing = false;
                        bShootingStar = false;
                        bFinishShooting = true;
                        trails.clear();
                    }
                }
            }
            else
            {
                //                curPos.z += 1;
                //                if (curPos.z > 1500)
                //                    curPos.z = initialdestPos.z - ofRandom(2000, 2500);
            }
        }
        
        checkShoot();
    }
    
    void draw()
    {
        if (bShootingStar && bMainmovement)
        {
            ofPushStyle();
            ofSetColor(col, getAlphaInt());
            ofCircle(curPos, rad);
            for (int i = 0; i < trails.size(); i++)
            {
                int alpha = ofMap(i, 0, trails.size(), 0, 60, true);
                ofSetColor(col, alpha);
                ofCircle(trails.at(i), rad);
            }
            ofPopStyle();
        }
        else
        {
            //            ofCircle(curPos, rad);
        }
    }
    
    void checkShoot()
    {
        int rdm = ofRandom(numStars * 3);
        if (rdm < 30)
            bShootingStar = true;
    }
    
    ofPoint initialPos;
    ofPoint initialdestPos;
    ofPoint curPos;
    ofPoint shootDestPos;
    ofPoint shootBasePos;
    ofColor col;
    float rad;
    
    float initializeTime;
    bool bInitialized;
    bool bMainmovement;
    float shoothingTime;
    bool bShootingStar;
    bool bShoothing;
    bool bFinishShooting;
    
    deque<ofPoint> trails;
};


class GalaxyOfStarState : public BaseState
{
public:
    
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        scrn.allocate(SVG_WIDTH, SVG_HEIGHT);
    }
    
    void stateEnter()
    {
        reset();
        getSharedData().bDefaultBlend = true;
    }
    
    void update()
    {
        BaseState::update();
        
        scrn.begin();
        ofClear(0);
        glEnable(GL_DEPTH_TEST);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i = 0; i < stars.size(); i++)
        {
            stars.at(i).update();
            stars.at(i).draw();
        }
        ofDisableBlendMode();
        glDisable(GL_DEPTH_TEST);
        scrn.end();
        scrn.readToPixels(*colorPixels);
        tex->loadData(colorPixels->getPixels(), SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
        
        int finShootingStar = 0;
        for (int i = 0; i < stars.size(); i++)
        {
            if (stars.at(i).bFinishShooting)
            {
                finShootingStar++;
            }
            else
            {
                break;
            }
        }
        if (finShootingStar == stars.size())
            reset();
    }
    
    void draw()
    {
        BaseState::draw();
        sharedData->location.drawLed();
    }
    
    string getName()
    {
        return "GalaxyOfStarState";
    }
    
    void stateExit()
    {
        getSharedData().bDefaultBlend = false;
    }

private:
    
    void reset()
    {
        ofColor col;
        int rdm = ofRandom(3);
        if (rdm == 0)
            col = ofColor::red;
        else if (rdm == 1)
            col = ofColor::cyan;
        else
            col = ofColor::green;
        
        stars.clear();
        for (int i = 0; i < numStars; i++)
        {
            ofColor thisCol = col;
            if (0 == i % 10)
            {
                int rdm2 = ofRandom(3);
                if (rdm2 == 0)
                    thisCol = ofColor::red;
                else if (rdm2 == 1)
                    thisCol = ofColor::cyan;
                else
                    thisCol = ofColor::green;
            }
            Star star;
            star.setup(thisCol);
            stars.push_back(star);
        }
    }
    
    ofTexture* tex;
    ofPixels* colorPixels;
    ofFbo scrn;
    
    vector<Star> stars;
};

#endif
