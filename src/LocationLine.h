//
//  Header.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 10/23/12.
//
//

#ifndef motherfarmLED_Header_h
#define motherfarmLED_Header_h
#include "LedNord.h"
#include "ofxGrabCam.h"

#define RED 1
#define BLUE 2
#define SVG_SCALE 1.65
#define SVG_OFFSET_X -250
#define SVG_OFFSET_Y 10
class LocationLine
{
public:
    vector<LedNord> nords;
    int dmxindex, group, lineIdInGroup;
    float offsetScale;
    ofColor color;
    
    virtual void addNord(ofPoint p)
    {
        if ( isAddible(p))
            nords.push_back(LedNord(nords.size()+1, p));
    }
    
    virtual void debugDraw()
    {
        for ( int i = 0; i < nords.size(); i++ )
        {
            ofPushMatrix();
            ofTranslate(nords[i].vec3.x, nords[i].vec3.y, nords[i].vec3.z);
            billboard();
            if ( nords[i].color != ofColor::black)
            {
                ofSetColor(nords[i].color);
            }
            else
            {
                ofSetColor(nords[i].color);
            }
            ofCircle(0,0,0, 0.1);
            ofPopMatrix();

        }
    }
    
    virtual void draw2D()
    {
        for ( int i = 0; i < nords.size(); i++ )
        {
            ofPushMatrix();
//            ofSetColor(nords[i].color);
            ofSetHexColor(0xffffff);
            ofTranslate(nords[i].pos.x, nords[i].pos.y, 0);
            ofCircle(0,0, 3);
            ofPopMatrix();
        }
    }
    
    virtual void updateColor( ofPixels * pix, ofPoint texOffset, int width, int height)
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
            }
        }
    }
    
    virtual void checkPos(ofxGrabCam &cam)
    {
        for ( int i = 0; i < nords.size(); i++ )
        {
            nords[i].vec3 = cam.getPosition3f(nords[i].pos.x * SVG_SCALE + SVG_OFFSET_X,
                                              nords[i].pos.y * SVG_SCALE + SVG_OFFSET_Y);
        }
    }
    
    virtual bool isAddible(ofPoint p)
    {
        bool result = true;
        for ( int i = 0; i < nords.size() && result; i++ )
        {
            if (p.distance(nords[i].pos) < 3)
            {
                result = false;
            }
        }
        return result;
    }
    
    virtual int getNordsNum()
    {
        return nords.size();
    }
    
    //sort
    bool operator <(const LocationLine& m1) const
    {
        bool result;
        if ( group == m1.group)
        {
            if( lineIdInGroup == m1.lineIdInGroup)
            {
                result = dmxindex < m1.dmxindex;
            }
            else
            {
                result = lineIdInGroup < m1.lineIdInGroup;
            }
        }
        else
        {
            result = group < m1.group;
        }
        return result;
    }
                
    bool operator >(const LocationLine& m1) const
    {
        bool result;
        if ( group == m1.group)
        {
            if( lineIdInGroup == m1.lineIdInGroup)
            {
                result = dmxindex > m1.dmxindex;
            }
            else
            {
                result = lineIdInGroup > m1.lineIdInGroup;
            }
        }
        else
        {
            result = group > m1.group;
        }
        return result;
    }

    string getString()
    {
        string lineIdStr = lineIdInGroup == BLUE ? "blue" : "red";
        string result = ofToString(group)+"["+lineIdStr+"]-"+ofToString(dmxindex);
        return result;
    };
                
    // http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20110221
    
    static inline void billboard()
    {
        GLfloat m[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, m);
        
        float inv_len;
        
        m[8] = -m[12];
        m[9] = -m[13];
        m[10] = -m[14];
        inv_len = 1. / sqrt(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);
        m[8] *= inv_len;
        m[9] *= inv_len;
        m[10] *= inv_len;
        
        m[0] = -m[14];
        m[1] = 0.0;
        m[2] = m[12];
        inv_len = 1. / sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
        m[0] *= inv_len;
        m[1] *= inv_len;
        m[2] *= inv_len;
        
        m[4] = m[9] * m[2] - m[10] * m[1];
        m[5] = m[10] * m[0] - m[8] * m[2];
        m[6] = m[8] * m[1] - m[9] * m[0];
        
        glLoadMatrixf(m);
    }
};
#endif
