#pragma once

#include "ofMain.h"
#include "Location.h"
#include "ofxStateMachine.h"
#include "SharedData.h"
#include "BaseState.h"
#include "ParseState.h"
#include "TextureDevState.h"
#include "SingleColorWave.h"
#include "MultiColorWave.h"

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
    
        itg::ofxStateMachine<SharedData> stateMachine;
        SharedData *sharedData;
    
    ofFbo fbo;
    ofImage img;
};
