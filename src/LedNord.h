//
//  LedNord.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 10/23/12.
//
//

#ifndef motherfarmLED_LedNord_h
#define motherfarmLED_LedNord_h
class LedNord
{
public:
    int index;
    ofPoint pos;
    LedNord(int _index, ofPoint _pos)
    {
        index = _index;
        pos = _pos;
    }
};


#endif
