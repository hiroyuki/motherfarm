//
//  VineLineBranchingState.h
//  motherfarmLED
//
//  Created by Akira on 11/11/12.
//
//

#ifndef motherfarmLED_VineLineBranchingState_h
#define motherfarmLED_VineLineBranchingState_h

#include "BaseState.h"
#include "ofxXmlSettings.h"

const static int MAX_CIRCLE_SIZE = 15;
const float fadeDur = 1.0;

class VectorField
{
public:
    
    void setup(float ts1, float ts2)
    {
        fScaleMult = 0.01 * ts1;
        fSpeedMult = 0.0005 * ts2;
        
        float w = 500, h = 500;
        fNoiseMin = 1;
        fNoiseMax = 0;
        
        for(int x = 0; x < w; x++)
        {
            for(int y = 0; y < h; y++)
            {
                float c = ofNoise(x * fScaleMult, y * fScaleMult);
                fNoiseMin = min(c, fNoiseMin);
                fNoiseMax = max(c, fNoiseMax);
            }
        }
    }
    
    float force(float x, float y, float z, float fScaleMultExtra, float fSpeedMultExtra)
    {
        float f = fScaleMult * fScaleMultExtra;
        float f2 = fSpeedMult * fSpeedMultExtra;
        float c = ofMap(ofNoise(x*f, y*f, z + f2 * ofGetElapsedTimeMillis()),
                        fNoiseMin, fNoiseMax,
                        -0.2, 1.2);
        c = MAX(MIN(c, 1), 0);
        return c;
    }
    
private:
    
    float fNoiseMin, fNoiseMax;
    float fScaleMult, fSpeedMult;
    
};

class Circle
{
public:
    
    void setup(float tx, float ty, float t_rx)
    {
        x = tx;
        y = ty;
        r = t_rx;
        a = 100;
        rs = ofRandom(0.98, 0.99);
        rs2 = sqrt(rs);
        fNoiseSpeed = ofRandom(0.5, 10);
    }
    
    void draw(VectorField vectorField, ofColor col)
    {
        ofPushStyle();
        
        r *= rs;
        a *= rs2;
        c = int(150 * vectorField.force(x, y, 0, 1, 1));
        
        float fAngle = vectorField.force(x,y, 10, fNoiseSpeed, fNoiseSpeed) * PI * 2;
        x += cos(fAngle);
        y += sin(fAngle);
        
        ofFill();
        ofSetColor(col, 20);
        ofCircle(x, y, r);
        
        ofPopStyle();
    }
    
    float x, y, r;
    float rs;
    float rs2;
    float a;
    float fNoiseSpeed;
    int c;
};

class Berry
{
public:
    
    void setup(ofPoint _pos, ofColor _col)
    {
        col = _col;
        
        poss.clear();
        ofPoint pos = _pos;
        int rdm = ofRandom(2, 4);
        for (int i = 0; i < rdm; i++)
        {
            ofPoint newPos;
            newPos.x = pos.x + ofRandom(-20, 20);
            newPos.y = pos.y + ofRandom(-20, 20);
            poss.push_back(newPos);
            float rad = ofRandom(40, 25);
            rads.push_back(rad);
            int a = ofRandom(10, 70);
            alphas.push_back(a);
        }
        bDrawn = false;
        drawTime = 0;
        maxDraw = 15;
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        if (!bDrawn && drawTime < maxDraw)
        {
            drawTime++;
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(0, 0, -10);
            for (int i = 0; i < poss.size(); i++)
            {
                ofSetColor(col, alphas.at(i));
                int rad = ofMap(drawTime, 0, maxDraw, 0, rads.at(i), true);
                ofCircle(poss.at(i), rad);
            }
            ofPopMatrix();
            ofPopStyle();
            if (drawTime >= maxDraw)
            {
                bDrawn = true;
            }
        }
    }
    
    bool bDrawn;
    vector<ofPoint> poss;
    vector<float> rads;
    vector<int> alphas;
    int drawTime;
    int maxDraw;
    ofColor col;
};

class Branch
{
public:
    
    Branch()
    {
        vectorField.setup(1, 1);
    }
    
    void setup(int _numCircles, ofColor _baseCol, ofColor _sashiCol)
    {
        baseCol = _baseCol;
        sashiCol = _sashiCol;
        numCircles = _numCircles;
        oldX = x = ofRandom(0, SVG_WIDTH);
        oldY = y = ofRandom(0, SVG_HEIGHT);
        vx = vy = 0;
        seed = ofRandom(10);
        
        for (int i = 0; i < numCircles; i++)
        {
            Circle circle;
            circle.setup(0, 0, 0);
            circles.push_back(circle);
        }
        
        fHeadSpeed = 2;
        
        genTime = ofGetElapsedTimef();
        growTime = ofRandom(4.5, 2.0);
        
        berry.setup(ofPoint(x, y), sashiCol);
    }
    
    void update()
    {
        if (berry.bDrawn)
        {
            if (x < 0 || x >= SVG_WIDTH || y < 0 || y >= SVG_HEIGHT)
                setup(numCircles, baseCol, sashiCol);
            
            if (x < 0)
                x += SVG_WIDTH;
            else if (x > SVG_WIDTH)
                x -= SVG_WIDTH;
            
            if (y < 0)
                y += SVG_HEIGHT;
            else if (y > SVG_HEIGHT)
                y -= SVG_HEIGHT;
            
            float fAngle = vectorField.force(x,y, 0, 1, 1) * PI * 2;
            x += cos(fAngle) * fHeadSpeed;
            y += sin(fAngle) * fHeadSpeed;
            
            if (ofGetElapsedTimef() - genTime < growTime)
                AddCircle(x, y);
        }
        
        berry.update();
    }
    
    void draw()
    {
        if (berry.bDrawn)
        {
            for(int i=0; i < numCircles; i++)
            {
                if(circles[i].r > 0.001)
                {
                    circles[i].draw(vectorField, baseCol);
                }
            }
        }
        
        berry.draw();
        
        oldX = x;
        oldY = y;
    }
    
private:
    
    void AddCircle(float x, float y)
    {
        circles[curCircle].setup(x, y, ofRandom(MAX_CIRCLE_SIZE));
        curCircle++;
        if(curCircle >= numCircles)
            curCircle = 0;
    }
    
    int curCircle = 0;
    vector<Circle> circles;
    Berry berry;
    float x, y, oldX, oldY;
    float vx, vy;
    float seed;
    
    VectorField vectorField;
    
    float fHeadSpeed;
    int numCircles;
    
    ofColor baseCol;
    ofColor sashiCol;
    
    float genTime;
    float growTime;
};

class Screen
{
public:

    void setup(vector<ofColor> _baseCols, vector<ofColor> _sashiCol)
    {
        numBranches = 10;
        numCircles = 1000;        
        
        reset(_baseCols, _sashiCol);
    }
    
    void update()
    {
        for (int i = 0; i < numBranches; i++)
        {
            branches.at(i).update();
        }
    }
    
    void draw()
    {   
        for (int i = 0; i < numBranches; i++)
        {
            branches.at(i).draw();
        }
    }
    
    void reset(vector<ofColor> _baseCols, vector<ofColor> _sashiCol)
    {
        baseCols = _baseCols;
        sashiCol = _sashiCol;
        
        branches.clear();
        for (int i = 0; i < numBranches; i++)
        {
            int rdm = ofRandom(baseCols.size());
            int rdmSashi = ofRandom(sashiCol.size());
            Branch branch;
            branch.setup(numCircles, baseCols.at(rdm), sashiCol.at(rdmSashi));
            branches.push_back(branch);
        }
    }
    
    
private:
    
    vector<Branch> branches;
    vector<ofColor> baseCols;
    vector<ofColor> sashiCol;
    
    int numBranches;
    int numCircles;
};

class VineLineBranchingState : public BaseState
{
    
public:
    
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;        
        
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        
        getStateSettingFromXML();
        
        for (int i = 0; i < 3; i++)
        {
            Screen* scrn = new Screen();
            scrn->setup(baseCols, sashiCols);
            screens.push_back(scrn);
        }
        reset();
    }
    
    void stateEnter()
    {
        getStateSettingFromXML();
        
        for (int i = 0; i < screens.size(); i++)
        {
            screens.at(i)->reset(baseCols, sashiCols);
        }
        
        getSharedData().bDefaultBlend = true;
    }
    
    void update()
    {
        BaseState::update();
        
        fbo.begin();
        ofSetColor(253, 253, 253);        
        fbo.draw(0, 0);
        for (int i = 0; i < screens.size(); i++)
        {
            screens.at(i)->update();
            screens.at(i)->draw();
        }
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
        return "VineLineBranchingState";
    }
    
    void stateExit()
    {
        getSharedData().bDefaultBlend = false;
        
        reset();
    }
    
    
private:
    
    void getStateSettingFromXML()
    {
        ofxXmlSettings xml;
        bool s = xml.loadFile("StateColor/" + getName() + ".xml");
        xml.pushTag("colors");
        int nTag = xml.getNumTags("baseCol");
        baseCols.clear();
        for (int i = 0; i < nTag; i++)
        {
            string rgb = xml.getValue("baseCol", "", i);
            vector<string> split = ofSplitString(rgb, ",");
            int r = ofToInt(split.at(0));
            int g = ofToInt(split.at(1));
            int b = ofToInt(split.at(2));
            ofColor col = ofColor(r, g, b);
            baseCols.push_back(col);
        }
        nTag = xml.getNumTags("sashiCol");
        sashiCols.clear();
        for (int i = 0; i < nTag; i++)
        {
            string rgb = xml.getValue("sashiCol", "", i);
            vector<string> split = ofSplitString(rgb, ",");
            int r = ofToInt(split.at(0));
            int g = ofToInt(split.at(1));
            int b = ofToInt(split.at(2));
            ofColor col = ofColor(r, g, b);
            sashiCols.push_back(col);
        }
    }
    
    void reset()
    {
        fbo.begin();
        ofClear(0);
        fbo.end();
    }
    
    ofFbo fbo;
    
    vector<Screen*> screens;
    ofTexture *tex;
    ofPixels * colorPixels;
    
    vector<ofColor> baseCols;
    vector<ofColor> sashiCols;
};

#endif
