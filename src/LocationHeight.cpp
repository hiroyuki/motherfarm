//
//  LocationHeight.cpp
//  motherfarmLED
//
//  Created by 堀 宏行 on 10/25/12.
//
//

#include "LocationHeight.h"
void LocationHeight::setup(string filename)
{
    // we need GL_TEXTURE_2D for our models coords.
//    ofDisableArbTex();
    cout << "start load model" << endl;
    if ( model.loadModel(filename, true) )
    {
        model.setPosition(-1782, -429.4, 1230);
        model.setScale(-0.05, -0.05, 0.05);
        cout << "succeed load model" << endl;
    }
    else
    {
        cout << "error load model" << endl;
    }
//    ofEnableArbTex();
	glEnable(GL_DEPTH_TEST);
}

void LocationHeight::setupScale()
{
    float scl = 1.2;
    model.setPosition(-1782*scl, -429.4*scl, 1230*scl);
    model.setScale(-1,-1, 1);
    
}

void LocationHeight::debugDraw()
{
//    ofTranslate(0, 0, -1000);
//    ofScale(-1, 1, 1);
    model.drawFaces();
}

float LocationHeight::getHeight(ofPoint point)
{
    float result;
    return result;
}