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


void Location::setup(ofPixels * pix)
{
//    colorTexture = colorTex;
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
    light.setPosition(255, 0, 0);
//    cam.setCursorDraw(true);
    cam.setFarClip(100);
    cam.setNearClip(0);
    placeLedOnWorld();
    light.setPosition(639, -891, 918);
    light.setAttenuation();
    cam.setFarClip(1000);
    cam.setNearClip(0);
    glDisable(GL_DEPTH_TEST);
    artnet.verbose = false;
    artnet.setup("192.168.11.100");
    tree.setup();
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
    
    tree.checkPos(cam);
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
        tree.debugDraw();
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
        tree.debugDraw();
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
        {
            ofSetHexColor(0xff0000);
            lines[ i ].draw2D();
        }
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

void Location::update()
{
    for( int i = 0; i < lines.size(); i++)
    {
        lines[ i ].updateColor(colorPix, ofPoint(TEX_OFFSET_X, TEX_OFFSET_Y), SVG_WIDTH, SVG_HEIGHT);
    }
    tree.update();
}

void Location::sendDmx(int showNo)
{
    for( int i = 0; i < dmxs.size(); i++)
    {
        {
            if ( dmxs[i].totalDataSize > 510)
            {
                cout << "error" << dmxs[i].totalDataSize << " " << dmxs[i].no << endl;
            }
            else
            {
                dmxs[i].copyData();
                if ( dmxs[i].no == showNo)
                {
                    Enttec enttec = dmxs[i];
                    int num = enttec.totalDataSize;
                    cout << num << endl;
                    for( int i = 0; i < num; i++)
                    {
                        unsigned char * datas = (unsigned char *)enttec.getData();
                        cout << (int)datas[i] << " ";
                        if ( (i+1) % 180 == 0) cout << endl;
                    }
                    cout << endl;
                }
                if ( dmxs[ i ].no != -1){
//                    cout << dmxs[i].getIpAddress() << " " << dmxs[i].totalDataSize << endl;
                    artnet.sendDmx(dmxs[i].getIpAddress(), dmxs[ i ].getData(), dmxs[i].totalDataSize);
                }
            }
        }
    }
    artnet.sendDmx(tree.getIpAddress(), tree.data, tree.getSize());
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
//            switch (i) {
//                default:
                    //createLine
                    
                    int group = color.r;
                    for( int i = 0; i < points.size(); i++)
                    {
                        line.addNord(points[i]);
                    }
#ifdef RISE
                    line.group = group;
#else
                    switch (group) {
                        case 235:
                            line.group = 1;
                            break;
                        case 245:
                            line.group = 2;
                            break;
                        case 195:
                            line.group = 3;
                            break;
                        case 205:
                            line.group = 4;
                            break;
                        case 145:
                            line.group = 5;
                            break;
                        case 135:
                            line.group = 19;
                            break;
                        case 225:
                            line.group = 18;
                            break;
                        case 215:
                            line.group = 17;
                            break;
                        case 185:
                            line.group = 16;
                            break;
                        case 175:
                            line.group = 15;
                            break;
                        case 165:
                            line.group = 13;
                            break;
                        case 155:
                            line.group = 14;
                            break;
                        case 115:
                            line.group = 12;
                            break;
                        case 125:
                            line.group = 11;
                            break;
                        case 105:
                            line.group = 7;
                            break;
                        case 95:
                            line.group = 8;
                            break;
                        case 75:
                            line.group = 9;
                            break;
                        case 85:
                            line.group = 10;
                            break;
                        default:
                            line.group = -1;
                            break;
                    }
#endif
                    line.lineIdInGroup = color.g == 127 ? BLUE : RED;
                    line.dmxindex = color.g == 127 ? color.b : color.g;
                    
                    lines.push_back(line);
//                    break;
//            }
            totalNum += points.size();
        }
    }
#ifndef RISE
//    LocationLine line;
//    line.group = 6;
//    line.lineIdInGroup = 1;
//    line.dmxindex = 1;
//    for( int i = 0; i < 100; i++)
//    {
//        line.addNord(ofPoint(SVG_WIDTH / 100 * i, SVG_HEIGHT));
//    }
//    lines.push_back(line);
#endif
    std::sort(lines.begin(), lines.end());
    int totalBufferSize = 0;
    for ( int i = 0; i < lines.size(); i++ )
    {
//        cout << lines[i].group << " " << lines[i].getNordsNum() << " " << lines[i].dmxindex << endl;
        lines[i].allocateData();
        totalBufferSize += lines[ i ].getDataSize();
        if ( dmxs.size() > 0 && dmxs.back().no == lines[i].group)
        {
            //add line to DMX
            dmxs.back().addLine(lines[i]);
        }
        else
        {
            //create DMX
            dmxs.push_back(Enttec(lines[i].group));
            dmxs.back().addLine(lines[i]);
        }
    }
    for( int i = 0; i < dmxs.size(); i++ )
    {
        dmxs[i].allocateData();
    }
}
