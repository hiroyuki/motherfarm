//
//  Location.cpp
//  motherfarmLED
//
//  Created by 堀 宏行 on 10/23/12.
//
//

#include "Location.h"
#include "SharedData.h"
ofImage img;

bool drawLine = false;


void Location::setup(ofTexture * colorTex, ofPixels * pix)
{
    colorTexture = colorTex;
    colorPix = pix;
#ifdef RISE
    loadSVG("1109_rise.svg");
#else
    loadSVG("dmx_mother_farm.svg");
#endif
#ifndef RISE
    mapHeight.setup("4_3.dae");
#endif
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    glEnable(GL_DEPTH_TEST);
    light.enable();
    light.setAmbientColor(ofFloatColor(0.2,0.2,0.2));
//    light.setDiffuseColor(ofFloatColor(0.1, 0.1, 0.1));
    light.setPosition(0, 0, 0);
//    cam.setCursorDraw(true);
    cam.setFarClip(100);
    cam.setNearClip(0);
    placeLedOnWorld();
    light.setPosition(639, -891, 918);
    light.setAttenuation();
    cam.setFarClip(1000);
    cam.setNearClip(0);
    glDisable(GL_DEPTH_TEST);
}
void Location::update()
{
    for( int i = 0; i < lines.size(); i++)
    {
        lines[ i ].updateColor(colorPix, ofPoint(TEX_OFFSET_X, TEX_OFFSET_Y), SVG_WIDTH, SVG_HEIGHT);
    }
}

void Location::setCameraPos( float x, float y, float z)
{
    cam.setPosition(x, y, z);
    cam.lookAt(ofVec3f(0));
}

void Location::setLight(float ambient_r, float ambient_g, float ambient_b, float diffuse_r, float diffuse_g, float diffuse_b)
{
    light.setAmbientColor(ofFloatColor(ambient_r, ambient_g, ambient_b));
    light.setDiffuseColor(ofFloatColor(diffuse_r, diffuse_g, diffuse_b));
}

void Location::exportXML()
{
    
}

void Location::placeLedOnWorld()
{
    ofDisableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
    ofFbo::Settings settings;
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
    settings.useDepth = true;
    settings.depthStencilInternalFormat = true;
    settings.depthStencilAsTexture = true;
    settings.useStencil = true;
    depthBufferFbo.allocate(settings);
    depthBufferFbo.begin();
    ofClear(0);
    ofSetHexColor(0xffffff);
    cam.begin();
    ofPushMatrix();
    mapHeight.debugDraw();
    ofPopMatrix();
    for (int i = 0; i < lines.size(); i++) {
        lines[ i ].checkPos(cam);
    }
    cam.end();
    depthBufferFbo.end();
}


void Location::debugDraw()
{
    ofSetHexColor(0xffffff);
    {
        cam.begin();
        mapHeight.debugDraw();
        ofDisableLighting();
        ofSetColor(255, 255, 255, 40);
        for (int i = 0; i < lines.size(); i++) {
            lines[ i ].debugDraw();
        }
        ofEnableLighting();
        cam.end();
    }
}

void Location::drawLed()
{
    ofSetHexColor(0xffffff);
    {
        cam.begin();
        ofDisableLighting();
        ofSetColor(255, 255, 255, 40);
        for (int i = 0; i < lines.size(); i++) {
            lines[ i ].debugDraw();
        }
        ofEnableLighting();
        cam.end();
    }
}

void Location::draw2dLine()
{
//    colorTexture->draw(ofPoint());
    ofPushMatrix();
    ofTranslate(TEX_OFFSET_X, TEX_OFFSET_Y);
    for( int i = 0; i < lines.size(); i++)
    {
        lines[ i ].draw2D();
    }
    ofPopMatrix();
}

void Location::changeLightPos(int _x, int _y, int _z)
{
    light.setPosition(_x, _y, _z);
}

void Location::drawDebugLine()
{
    drawLine ^= true;
}

LocationLine* Location::getLine(int group, int lineIdInGroup, int dmxIndex)
{
    LocationLine* line = NULL;
    for( int i = 0; i < lines.size() && line == NULL; i++ )
    {
        if ( group == lines[i].group &&
                lineIdInGroup == lines[i].lineIdInGroup &&
                dmxIndex == lines[i].dmxindex) line = &lines[i];
    }
    return line;
}

void Location::loadSVG(string filename)
{
    svg.load(filename);
    int totalNum = 0;
    for (int i = 0; i < svg.getNumPath(); i++)
    {
        ofPath &p = svg.getPathAt(i);
        
        vector<ofPolyline>& _lines = p.getOutline();
        {
#ifdef RISE
            RiseLocationLine line;
#else
            LocationLine line;
#endif
            vector<ofPoint>& points = _lines[0].getVertices();
            ofColor color = p.getStrokeColor();
            line.color = color;
            switch (i) {
                default:
                    //createLine
                    
                    int group = color.r;
                    for( int i = 0; i < points.size(); i++)
                    {
                        line.addNord(points[i]);
                    }
                    line.group = group;
                    line.lineIdInGroup = color.g == 127 ? BLUE : RED;
                    cout << line.group << " " << line.lineIdInGroup << endl;
                    line.dmxindex = color.g == 127 ? color.b : color.g;
                    lines.push_back(line);
                    break;
            }
            totalNum += points.size();
        }
    }
    std::sort(lines.begin(), lines.end());
}
