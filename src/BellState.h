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
#define RECT_NUM_X 100.f
#define RECT_NUM_Y 35.f
#define CENTERX_INTERVAL 20
#define ANGLE_RESOLUTION 0.01
#define TOCENTER_NUM 20
enum RECT_COLOR
{
    R_BLUE, R_YELLOW
};

class ColorRect
{
public:
    float x, y;
    int r,g,b;
    int seedx, seedy;
    RECT_COLOR color;
    int size;
    int initialMs;
    ColorRect(int sx, int sy)
    {
        seedx = sx;
        seedy = sy;
        size = 0;
        initialMs = ofGetElapsedTimeMillis();
    }
    
    void update()
    {
        float t = ofGetElapsedTimeMillis() * 0.0001;
        r = (color == R_YELLOW) ? ofNoise( seedx * seedy * t) * 50.f + 255.f
                                    :ofNoise( seedx * t) * 150.f;
        g = (color == R_YELLOW) ? ofNoise( seedx * seedy * t) * 50.f + 255.f
                                    :ofNoise( seedy * t) * 150.f;
        b = (color == R_BLUE) ? ofNoise( seedx * seedy * t) * 150.f + 105.f
                                    : 0.f;
    }
    
    void draw(float alpha)
    {
        ofSetColor(r, g, b, alpha*255.f);
        ofRect(x, y, size, size);
    }
    
    void drawBell(float alpha)
    {
        ofSetColor(r, g, b, alpha*255.f);
        ofRect(x, y + (ofNoise(seedx + ofGetElapsedTimeMillis() * 0.001f)-0.5f) * SVG_HEIGHT*2.f, size, size);
    }
};
class BellState : public BaseState
{
public:
    ofTexture *tex;
    ofPixels * colorPixels;
    int longestLen;
    vector<ColorRect> rects;
    vector<CircleToCenter> centers;
    float pastAngle;
    void setup()
    {
        BaseState::setup();
        tex = sharedData->tex;
        colorPixels = sharedData->colorPixels;
        longestLen = sqrt(pow(SVG_WIDTH, 2.f) + pow(SVG_HEIGHT, 2.f));
        fbo.allocate(SVG_WIDTH, SVG_HEIGHT, GL_RGBA32F_ARB);
        fbo.begin();ofClear(0);fbo.end();
        assignRect();
        pastAngle = 0;
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
        assignRect();
        show();
    }
    
    void assignRect()
    {
        rects.clear();
        for( float i = 0; i <= longestLen / RECT_NUM_X; i++)
//        float i = longestLen / 2.f / RECT_NUM_X;
        {
            for( int j = 0; j <= longestLen / RECT_NUM_Y; j++)
            {
                rects.push_back(ColorRect(i, j));
                rects.back().color = R_BLUE;
                rects.back().x = i * RECT_NUM_X;
                rects.back().y = j * RECT_NUM_Y;
                rects.back().size = 15;
            }
        }
        for ( float i = 0; i < SVG_WIDTH; i += CENTERX_INTERVAL)
        {
            rects.push_back(ColorRect(i, i));
            rects.back().color = R_YELLOW;
            rects.back().x = i;
            rects.back().size = 10;
            rects.back().y = (SVG_HEIGHT - rects.back().size)>>1;
        }
        for ( int i = 0; i < TOCENTER_NUM; i++)
        {
            centers.push_back(CircleToCenter(i, false));
            centers.back().init(SVG_WIDTH, SVG_HEIGHT, longestLen);
        }
    }
    
    void update()
    {
        BaseState::update();
        ofDisableLighting();
        ofEnableAlphaBlending();
        glDisable(GL_DEPTH_TEST);
        fbo.begin();
        ofSetColor(240*alpha, 240*alpha, 240*alpha, 240*alpha);
        fbo.draw(0, 0);
        ofSetHexColor(0xffffff);
        ofPushMatrix();
        ofTranslate( (SVG_WIDTH-longestLen)>>1,(SVG_HEIGHT-longestLen)>>1);
        ofTranslate(longestLen/2, longestLen/2);
        float angle = ofGetElapsedTimeMillis()/ 10.f;

        ofRotate(ofGetElapsedTimeMillis()/ 25.f, 0, 0, 1);
        for( int i = 0; i < rects.size(); i++ )
        {
            if ( rects[i].color == R_BLUE)
            {
                ofPushMatrix();
                ofTranslate(-longestLen / 2, -longestLen / 2);
                rects[i].update();
                rects[i].draw(alpha);
                ofPopMatrix();
            }
        }
        float pastAngle = angle;
        ofPopMatrix();
        ofPushMatrix();
        for( int i = 0; i < rects.size(); i++ )
        {
            if ( rects[i].color == R_YELLOW)
            {
                ofPushMatrix();
                rects[i].update();
                rects[i].drawBell(alpha);
                ofPopMatrix();
            }
        }
        ofPopMatrix();
        ofPushMatrix();
        for( int i = 0; i < centers.size(); i++ )
        {
            ofPushMatrix();
                centers[i].update();
                centers[i].draw(alpha);
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
