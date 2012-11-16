//
//  StatusManagement.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/16/12.
//
//

#ifndef motherfarmLED_StatusManagement_h
#define motherfarmLED_StatusManagement_h
#include "ofxHttpUtils.h"
#include "ofxSimplestDateParse.h"
#include "ofxDate.h"
//#include "SharedData.h"
#define CHECK_INTERVAL 10000
#define LOAD_INTERVAL 1000
class myDate
{
public:
    ofxDate date;
    int timesec =0;
    void setTimesec( int h, int mm, int sec)
    {
        timesec = (sec + mm * 60 + h * 60 * 60) * 1000;
    }
    
    void setDate( int y, int m, int d)
    {
        date.setYear(y);
        date.setMonth(m);
        date.setDay(d);
    }
};

class Action
{
public:
    myDate actionDate;
    int type;
    bool executed = true;
};

class StatusManagement
{
public:
    int lastCheckMs, lastLoadMs;
    ofxHttpForm form;
    ofxHttpUtils httpUtils;
    int currentServerTime;
    ofxXmlSettings dateXML;
    myDate checkedDate;
    myDate currentDate;
    Action action;
    string lastExecuted;
    SharedData *sharedData;
    
    StatusManagement()
    {
        lastLoadMs = lastCheckMs = ofGetElapsedTimeMillis();
        ofAddListener(httpUtils.newResponseEvent,this,&StatusManagement::newResponse);
        httpUtils.start();
        httpUtils.setVerbose(false);
    }
    
    void init(SharedData *_sharedData)
    {
        sharedData = _sharedData;
    }
    
    void update()
    {
        if ( ofGetElapsedTimeMillis() - lastLoadMs > LOAD_INTERVAL)
        {
            ofxHttpForm form;
            form.action="http://ex.rzm.co.jp/risexmas2012/data";
            form.method = OFX_HTTP_GET;
            httpUtils.addForm(form);
            lastLoadMs = ofGetElapsedTimeMillis();
        }
        if ( ofGetElapsedTimeMillis() - lastCheckMs > CHECK_INTERVAL)
        {
            ofxHttpForm form;
            form.action = "http://ex.rzm.co.jp/risexmas2012/now";
            form.method = OFX_HTTP_GET;
            httpUtils.addForm(form);
            lastCheckMs = ofGetElapsedTimeMillis();
        }
        else if ( currentDate.date == checkedDate.date && currentDate.timesec > 0 )
        {
            currentDate.timesec = checkedDate.timesec + (ofGetElapsedTimeMillis() - lastCheckMs);
        }
        if ( !action.executed && currentDate.timesec != 0 )
        {
            if ( currentDate.timesec > action.actionDate.timesec && currentDate.date >= action.actionDate.date)
            {
                cout << "execute!!!!!" << endl;
                sharedData->dt.eventName = "changeState";
                string stateName;
                sharedData->doNoise = false;
                switch( action.type)
                {
                    case 1:
                        stateName = "VineLineBranchingState";
                        break;
                    case 2:
                        stateName = "GalaxyOfStarState";
                        break;
                    case 3:
                        stateName = "BellState";
                        break;
                }
                sharedData->curState = sharedData->dt.nextState = stateName;
                ofNotifyEvent(sharedData->event.farmEvent, sharedData->dt, this);
                action.executed = true;
            }
        }
    }
    
    void newResponse(ofxHttpResponse & response)
    {
//        cout << response.url << endl;
        if ( response.url == "/risexmas2012/now")
        {
            dateXML.loadFromBuffer((string)response.responseBody);
            int y, m, d, h, mm, s;
            ofxSimplestDateParse::parse( dateXML.getValue("result:time", "error"),"%d-%d-%d %d:%d:%d", y, m, d, h, mm, s );
            checkedDate.setDate(y, m, d);
            checkedDate.setTimesec(h, mm, s);
            currentDate = checkedDate;
        }
        else if ( response.url == "/risexmas2012/data")
        {
            dateXML.loadFromBuffer((string)response.responseBody);
            int y, m, d, h, mm, s;
            ofxSimplestDateParse::parse( dateXML.getValue("result:time", "error"),"%d-%d-%d %d:%d:%d", y, m, d, h, mm, s );
            if ( currentDate.timesec == 0)
            {
                lastExecuted =  dateXML.getValue("result:time", "error");
            }
            else
            {
                if ( lastExecuted != dateXML.getValue("result:time", "error"))
                {
                    cout << "loaded new action" << endl;
                    action.actionDate.setDate(y, m, d);
                    action.actionDate.setTimesec(h, mm, s);
                    action.actionDate.timesec += 10000;
                    sharedData->doNoise = true;
                    action.type = dateXML.getValue("result:type", -1);
                    action.executed = false;
                    lastExecuted =  dateXML.getValue("result:time", "error");
                }
            }
        }
    }
};


#endif
