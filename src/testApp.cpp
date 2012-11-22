#include "testApp.h"
#include "ofxSimpleGuiToo.h"


int startHour, startMin, endHour, endMin;
bool isAvailable;
bool isScreenSaver;
bool compell = true;
//--------------------------------------------------------------
void testApp::setup(){
//    ofSetVerticalSync(true);
    ofSetFrameRate(40);
    sharedData = new SharedData();
    sharedData->setup();
    //    stateMachine.addState(new BaseState());
    states.push_back(new TextureDevState(sharedData));
    states.back()->changeActive(true);
    states.push_back(new SingleColorWave(sharedData));
    states.push_back(new SRandomNoiseState(sharedData));
    states.push_back(new MovieState(sharedData));
    states.push_back(new MultiColorWave(sharedData));
    states.push_back(new CircleColorState(sharedData));
    states.push_back(new CircleToCenterState(sharedData));
    states.push_back(new CircleToOutState());
    states.back()->setSharedData(sharedData);
    states.push_back(new DanganState());
    states.back()->setSharedData(sharedData);
    states.push_back(new ScaleCircleState(sharedData));
    states.push_back(new BellState(sharedData));
    states.push_back(new NoiseState(sharedData));
    states.push_back(new WindowState(sharedData));
    states.push_back(new GradualChangerState(sharedData));
    states.push_back(new ClickState(sharedData));
    states.push_back(new GradualChangeGrayState(sharedData));
    states.push_back(new RandomNoiseState(sharedData));
    states.push_back(new FillCircleState(sharedData));
    states.push_back(new FortimeLoopState(sharedData));
//    states.back()->changeActive(true);
    
    durationSender.setup("192.168.11.99", 12346);
    if ( false )
    {
        for( int i = 0; i < states.size(); i++)
        {
            ofxOscMessage m;
            m.setAddress("/duration/addtrack");
            m.addStringArg("switches");
            m.addStringArg("state " + states[i]->getName());
            durationSender.sendMessage(m);
            ofSleepMillis(100);
        }
    }
    if ( false )
    {
        ofxOscMessage m;
        m.setAddress("/duration/addtrack");
        m.addStringArg("switches");
        m.addStringArg("state " + states.back()->getName());
        durationSender.sendMessage(m);
    }
    
    mainFbo.allocate(SVG_WIDTH, SVG_HEIGHT);
    xml.loadFile("setting.xml");
    startHour = xml.getValue("start:hour", 0);
    startMin = xml.getValue("start:minutes", 0);
    endHour = xml.getValue("end:hour", 0);
    endMin = xml.getValue("end:minutes", 0);
    ofAddListener(sharedData->event.farmEvent, this, &testApp::eventListener);
    isAvailable = ( ofGetHours() >= startHour && ofGetHours() < endHour);
    
    
    for( int i=0; i < states.size(); i++)
    {
        states[i]->setup();
    }
    mainFbo.begin();ofClear(0);mainFbo.end();
    statusManager.init(sharedData);
    sharedData->sendStart();
}

//--------------------------------------------------------------
void testApp::update(){
    if ( ofGetHours() == endHour - 1)
    {
        if ( ofGetMinutes() > 30)
        {
            isScreenSaver = true;
        }
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    if ( !isAvailable && !compell)
    {
        states[0]->update();
        mainFbo.begin();
        ofClear(0);
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        states[0]->draw();
    }
    else if ( isScreenSaver && !compell)
    {
        states[1]->update();
        sharedData->curveA = 1;
        states[2]->update();
        mainFbo.begin();
        ofClear(0);
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        states[1]->draw();
        states[2]->draw();
    }
    else
    {
        for( int i=0; i < states.size(); i++)
        {
            if ( states[i]->getIsActive() || states[i]->bdoUpdate() )
            {
                states[i]->update();
            }
        }
        mainFbo.begin();
        ofClear(0);
        ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        for( int i=0; i < states.size(); i++)
        {
            if ( states[i]->getIsActive() || states[i]->bdoUpdate() )
            {
    //            states[i]->update();
                states[i]->draw();
            }
        }
    }
    ofDisableBlendMode();
    mainFbo.end();
    mainFbo.readToPixels(*sharedData->colorPixels);
    sharedData->update();
    sharedData->sendDmx();

}
//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0, 0, 0);
    {
        glEnable(GL_DEPTH_TEST);
        mainFbo.draw(ofGetWidth() - mainFbo.getWidth() - 10, 10);
        if ( sharedData->showParse )
        {
            ofEnableLighting();
            sharedData->location.debugDraw();
        }
        sharedData->location.drawLed();
    }
    ofEnableAlphaBlending();

    if (sharedData->bDefaultBlend)
        glBlendFunc(GL_ONE, GL_NONE);
    glDisable(GL_DEPTH_TEST);
    ofNoFill();
    ofRect(ofGetWidth() - mainFbo.getWidth() - 10, 10, SVG_WIDTH, SVG_HEIGHT);
    ofFill();
    
    if ( sharedData->show2D )
    {
        glDisable(GL_DEPTH_TEST);
        ofDisableLighting();
        ofSetHexColor(0xffffff);
        sharedData->location.draw2dLine();
        ofSetHexColor(0xffffff);
        ofNoFill();
        ofRect(0, 0, SVG_WIDTH, SVG_HEIGHT);
        ofFill();
    }
    sharedData->windManager.debugDraw();
    gui.draw();
}

void testApp::eventListener( FarmEventData& data)
{
//    if ( data.eventName == "showState")
//    {
//        cout << "change state " << data.nextState << endl;
//        stateMachine.changeState(sharedData->dt.nextState);
//    }
    if ( data.eventName == "changeState")
    {
        for( int i = 0; i < states.size(); i++ )
        {
            if ( data.stateName == states[i]->getName())
                states[i]->changeActive((bool) data.bActive);
        }
    }
//    if ( data.eventName == "showCur")
//    {
//        stateMachine.currentState->show();
//    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    switch (key) {
        case 'l':
//            sharedData->location.drawDebugLine();
            break;
//        case 'e':
//            location.exportLocation();
            break;
        case '1':
            sharedData->doNoise = 1;
            sharedData->noiseAlpha = 0;
            break;
        case '2':
            sharedData->doNoise = 2;
            sharedData->noiseAlpha = 1;
            break;
            
        default:
            break;
    }
    if(key>='0' && key<='9') {
		gui.setPage(key - '0');
		gui.show();
	} else {
		switch(key) {
			case ' ': gui.toggleDraw(); break;
			case '[': gui.prevPage(); break;
			case ']': gui.nextPage(); break;
			case 'p': gui.nextPageWithBlank(); break;
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}