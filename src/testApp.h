#pragma once

#include "ofMain.h"
#include "Location.h"
#include "SharedData.h"
#include "statesInc.h"
#include "ofxXmlSettings.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        void eventListener( FarmEventData& data);
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		virtual void exit(){
            sharedData->sendExit();
        }
    
        vector< BaseState*> states;
        SharedData *sharedData;
        StatusManagement statusManager;
        ofxXmlSettings xml;
        ofImage img;
        ofFbo mainFbo;
    ofxOscSender durationSender;
};
