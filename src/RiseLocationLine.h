//
//  RiseLocation.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/12/12.
//
//

#ifndef motherfarmLED_RiseLocation_h
#define motherfarmLED_RiseLocation_h
#include "LocationLine.h"

class RiseLocationLine : public LocationLine
{
public:

    void debugDraw()
    {
        
        for ( int i = 0; i < nords.size(); i++ )
        {
            ofPushMatrix();
            ofTranslate(nords[i].pos.x -300, nords[i].pos.y - 300, -600);
            billboard();
            if ( nords[i].color != ofColor::black)
            {
                ofSetColor(nords[i].color);
            }
            else
            {
                ofSetColor(nords[i].color);
            }
            ofCircle(0,0,0, 1);
            ofPopMatrix();
            
        }
    }
    
    void draw2D()
    {
        for ( int i = 0; i < nords.size(); i++ )
        {
            ofPushMatrix();
            ofSetColor(nords[i].color);
            ofTranslate(nords[i].pos.x, nords[i].pos.y, 0);
            ofCircle(0,0, 1);
            ofPopMatrix();
        }
    }
    
    void updateColor( ofPixels * pix, ofPoint texOffset, int width, int height)
    {
        for ( int i = 0; i < nords.size(); i++ )
        {
            int pointx = nords[ i ].pos.x + texOffset.x;
            int pointy = nords[ i ].pos.y + texOffset.y;
            if ( pointy * width + pointx < pix->getWidth() * pix->getHeight() )
            {
                nords[ i ].color.r = pix->getPixels()[ ( pointy * width + pointx ) * 4];
                nords[ i ].color.g = pix->getPixels()[ ( pointy * width + pointx ) * 4 + 1];
                nords[ i ].color.b = pix->getPixels()[ ( pointy * width + pointx ) * 4 + 2];
                data[ i * 3 ] = pix->getPixels()[ ( pointy * width + pointx ) * 4];
                data[ i * 3 + 1] = pix->getPixels()[ ( pointy * width + pointx ) * 4 + 1];
                data[ i * 3 + 2] = pix->getPixels()[ ( pointy * width + pointx ) * 4 + 2];
            }
        }
    }
    
    void checkPos(ofxGrabCam &cam)
    {
        for ( int i = 0; i < nords.size(); i++ )
        {
            nords[i].vec3 = cam.getPosition3f(nords[i].pos.x * SVG_SCALE + SVG_OFFSET_X,
                                              nords[i].pos.y * SVG_SCALE + SVG_OFFSET_Y);
        }
    }
    
    string getString()
    {
        string lineIdStr = lineIdInGroup == BLUE ? "blue" : "red";
        string result = ofToString(group)+"["+lineIdStr+"]-"+ofToString(dmxindex);
        return result;
    };
};
#endif
