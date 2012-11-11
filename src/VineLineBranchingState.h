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

const static int MAX_CIRCLE_SIZE = 10;

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
//        ofSetColor(ofColor::green, a);
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

class Branch
{
public:
    
    Branch()
    {
        vectorField.setup(1, 1);
    }
    
    void setup(int _numCircles, ofColor _baseCol)
    {
        baseCol = _baseCol;
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
    }
    
    void update()
    {
        if (x < 0 || x >= SVG_WIDTH || y < 0 || y >= SVG_HEIGHT)
            setup(numCircles, baseCol);
        
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
        AddCircle(x, y);
    }
    
    void draw()
    {
        for(int i=0; i < numCircles; i++)
        {
            if(circles[i].r > 0.001)
            {
                circles[i].draw(vectorField, baseCol);
            }
        }
        
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
    float x, y, oldX, oldY;
    float vx, vy;
    float seed;
    
    VectorField vectorField;
    
    float fHeadSpeed;
    int numCircles;
    
    ofColor baseCol;
};

class VineLineBranchingState : public BaseState
{
    
public:
    
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;        
        
        numBranches = 4;
        numCircles = 1000;
        
//        ofColor col1 = ofColor(0,143,196);
        ofColor col1 = ofColor(18,150,54);
        ofColor col2 = ofColor(18,150,54);
        cols.push_back(col1);
        cols.push_back(col2);
        
        for (int i = 0; i < numBranches; i++)
        {
            float rdm = ofRandomf();
            Branch branch;
            if (rdm < 0)
                branch.setup(numCircles, cols.at(0));
            else
                branch.setup(numCircles, cols.at(1));
            branches.push_back(branch);
        }
        
        scrn.allocate(SVG_WIDTH, SVG_HEIGHT);
        scrn.begin();
        ofClear(0);
        scrn.end();
    }
    
    void stateEnter()
    {
        scrn.begin();
        ofClear(0);
        scrn.end();
    }
    
    void update()
    {
        scrn.begin();
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for (int i = 0; i < numBranches; i++)
        {
            branches.at(i).update();
            branches.at(i).draw();
        }
        ofDisableBlendMode();
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
        return "VineLineBranchingState";
    }
    
    
private:
    
    int numBranches;
    int numCircles;
    vector<Branch> branches;
    
    ofFbo scrn;
    
    vector<ofColor> cols;
    
    ofTexture *tex;
    ofPixels * colorPixels;    
};

#endif
