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
#include "ofxXmlSettings.h"

const int numStars = 150;
const float initialPositioningDur = 0.1;
const float shootingDur = 7.0;
const int numMaxTrail = 50;

const int minDist = -2500;
const int maxDist = -5500;

class Star : public ofxFadableBase
{
public:
    
    void setup(ofColor _col)
    {
        initialdestPos = ofPoint(ofRandom(-SVG_WIDTH/2, SVG_WIDTH+SVG_WIDTH/2),
                                 ofRandom(-SVG_HEIGHT/2, SVG_HEIGHT+SVG_HEIGHT/2),
                                 ofRandom(minDist, maxDist));
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
                    float factor = ofxTweenLite::tween(0.0, 1.0, mappedProgress, OF_EASE_LINEAR_INOUT);
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
                curPos.z += 1;
                if (curPos.z > -500)
                    curPos.z = /*initialdestPos.z + */ofRandom(minDist, maxDist);
            }
        }
        
        checkShoot();
    }
    
    void draw(float palpha)
    {
        if (bShootingStar && bMainmovement)
        {
            ofPushStyle();
            int a = ofMap(curPos.z, maxDist, -500, -100, 255, true);
            ofSetColor(col, a*palpha);
            ofCircle(curPos, rad);
            if (a > 0)
            {
                //            for (int i = 0; i < trails.size(); i++)
                //            {
                //                int alpha = ofMap(i, 0, trails.size(), 0, a, true);
                ////                alpha += a;
                //                ofSetColor(col, alpha);
                //                ofCircle(trails.at(i), rad);
                //            }
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
        scrn.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
    }
    
    void stateEnter()
    {
        getStateSettingFromXML();
        reset();
        getSharedData().bDefaultBlend = true;
        
        scrn.begin();
        ofClear(0);
        scrn.end();
        
        
        BaseState::stateEnter();
    }
    
    void update()
    {
        ofEnableAlphaBlending();
        
        BaseState::update();
        
        scrn.begin();
        ofClear(0);
        
        ofPushStyle();
        ofSetColor(120, 0, 0, alpha*255);
        ofRect(0, 0, SVG_WIDTH, SVG_HEIGHT);
        ofPopStyle();
        
        glEnable(GL_DEPTH_TEST);
        for (int i = 0; i < stars.size(); i++)
        {
            stars.at(i).update();
            stars.at(i).draw(alpha);
        }
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
    
    void stateExit()
    {
        getSharedData().bDefaultBlend = false;
    }
    
private:
    
    void reset()
    {
        stars.clear();
        for (int i = 0; i < numStars; i++)
        {
            int rdm = ofRandom(colsFromXML.size());
            ofColor thisCol = colsFromXML.at(rdm);
            
            Star star;
            star.setup(thisCol);
            stars.push_back(star);
        }
    }
    
    void getStateSettingFromXML()
    {
        ofxXmlSettings xml;
        bool s = xml.loadFile("StateColor/" + getName() + ".xml");
        xml.pushTag("colors");
        int nTag = xml.getNumTags("col");
        colsFromXML.clear();
        for (int i = 0; i < nTag; i++)
        {
            string rgb = xml.getValue("col", "", i);
            vector<string> split = ofSplitString(rgb, ",");
            int r = ofToInt(split.at(0));
            int g = ofToInt(split.at(1));
            int b = ofToInt(split.at(2));
            ofColor col = ofColor(r, g, b);
            colsFromXML.push_back(col);
        }
    }
    ofFbo scrn;
    
    vector<Star> stars;
    vector<ofColor> colsFromXML;
};

#endif
