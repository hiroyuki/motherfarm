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

#define RED 1
#define BLUE 2

class LocationLine
{
public:
    vector<LedNord> nords;
    int dmxindex, group, lineIdInGroup;
    void addNord(ofPoint p)
    {
        if ( isAddible(p))
            nords.push_back(LedNord(nords.size()+1, p));
    }
    
    void appendNords( vector<LedNord> nords)
    {
        
    }
    
    void prependNords( vector<LedNord> nords)
    {
        
    }
    
    void debugDraw()
    {
        ofPushMatrix();
        ofSetHexColor(0x009999);
        for ( int i = 0; i < nords.size(); i++ )
        {
            ofCircle(nords[i].pos.x, nords[i].pos.y, 5);
        }
        ofPopMatrix();
    }
    
    bool isAddible(ofPoint p)
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
    
    int getNordsNum()
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
};
#endif
