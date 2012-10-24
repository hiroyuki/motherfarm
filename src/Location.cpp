//
//  Location.cpp
//  motherfarmLED
//
//  Created by 堀 宏行 on 10/23/12.
//
//

#include "Location.h"

float step;
void Location::setup()
{
    svg.load("dmx_line.svg");
    for (int i = 0; i < svg.getNumPath(); i++)
    {
        ofPath &p = svg.getPathAt(i);
        
        vector<ofPolyline>& _lines = p.getOutline();
        {
            LocationLine line;
            vector<ofPoint>& points = _lines[0].getVertices();
            ofColor color = p.getStrokeColor();
            switch (i) {
//                case 46:
//                    cout << "add to 30 " << endl;
//                    for( int i = 0; i < points.size(); i++)
//                    {
//                        if ( getLine(30) != NULL)
//                        {
//                            getLine(30)->addNord(points[i]);
//                        }
//                    }
//                    break;
//                case 43:
//                    cout << "add to 35 " << endl;
//                    for( int i = 0; i < points.size(); i++)
//                    {
//                        if ( getLine(35) != NULL)
//                            getLine(35)->addNord(points[i]);
//                    }
//                    break;
                default:
                    //createLine
                    int red = color.r;
                    int group = 9 - (255 - red)/20;
                    for( int i = 0; i < points.size(); i++)
                    {
                        line.addNord(points[i]);
                    }
                    line.group = group;
                    line.lineIdInGroup = color.g == 127 ? BLUE : RED;
                    line.dmxindex = color.g == 127 ? color.b : color.g;
                    lines.push_back(line);
                    break;
            }
        }
    }
    std::sort(lines.begin(), lines.end());
    for ( int i = 0; i< lines.size(); i++)
    {
        cout << lines[i].getString() << endl;
    }
}

void Location::update()
{
	step += 0.001;
	if (step > 1)
	{
		step -= 1;
	}
}

void Location::exportXML()
{
    
}

void Location::debugDraw()
{
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofTranslate(-svg.getWidth() / 2, -svg.getHeight() / 2);
	if(ofGetMousePressed()) {
        for(int i = 0; i < lines.size(); i++)
        {
            if(lines[i].dmxindex == 127) lines[i].debugDraw();
        }
	} else {
		svg.draw();
	}
	
	ofPopMatrix();
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
