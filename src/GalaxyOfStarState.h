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

const int numStars = 300;
const float initialPositioningDur = 0.1;
const float shootingDur = 5.0;
const int numMaxTrail = 100;

class Star
{
public:
    
    void setup()
    {
        initialdestPos = ofPoint(ofRandom(-SVG_WIDTH/2, SVG_WIDTH+SVG_WIDTH/2),
                                 ofRandom(-SVG_HEIGHT/2, SVG_HEIGHT+SVG_HEIGHT/2),
                                 ofRandom(100, -2500));
        initialPos = initialdestPos;
        initialPos.z += 1000;
        curPos = initialPos;
        shootDestPos = initialPos;
        shootDestPos.z += 3000;
        shootBasePos = shootDestPos;
        col = ofColor::white;
        rad = ofRandom(30, 10);
        
        initializeTime = 0.0;
        bInitialized = false;
        bMainmovement = false;
        shoothingTime = 0.0;
        bShootingStar = false;
        bShoothing = false;
    }
    
    void update()
    {
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
                    float factor = ofxTweenLite::tween(0.0, 1.0, mappedProgress, OF_EASE_EXPO_OUT);
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
                        trails.clear();
                    }
                }
            }
            else
            {
                curPos.z += 1;
                if (curPos.z > 1500)
                    curPos.z = initialdestPos.z - ofRandom(2000, 2500);
            }
        }
        
        checkShoot();
    }
    
    void draw()
    {
        if (bShootingStar && bMainmovement)
        {
            ofPushStyle();
            ofSetColor(0,150,216);
            ofCircle(curPos, rad);
            for (int i = 0; i < trails.size(); i++)
            {
                int alpha = ofMap(i, 0, trails.size(), 0, 60, true);
                ofSetColor(0,150,216, alpha);
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
        stars.clear();
        for (int i = 0; i < numStars; i++)
        {
            Star star;
            star.setup();
            stars.push_back(star);
        }
    }
    
    void update()
    {
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

private:
    
    ofTexture* tex;
    ofPixels* colorPixels;
    ofFbo scrn;
    
    vector<Star> stars;
};

#endif