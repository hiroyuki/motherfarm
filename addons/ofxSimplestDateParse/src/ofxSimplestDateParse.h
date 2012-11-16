//
//  ofxSimplestDateParse.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/16/12.
//
//

#ifndef motherfarmLED_ofxSimplestDateParse_h
#define motherfarmLED_ofxSimplestDateParse_h
class ofxSimplestDateParse
{
public:
    static void parse( string date, string fmt, int& y, int& m, int& d, int& h, int& mm, int& s)
    {
        sscanf(date.c_str(), fmt.c_str(), &y, &m, &d, &h, &mm, &s);
    }
};


#endif
