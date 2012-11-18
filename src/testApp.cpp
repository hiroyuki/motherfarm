#include "testApp.h"
int startHour, startMin, endHour, endMin;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(40);
    sharedData = &stateMachine.getSharedData();
    sharedData->setup();
//    stateMachine.addState(new BaseState());
    stateMachine.addState(new ParseState());
    stateMachine.addState(new TextureDevState());
    stateMachine.addState(new SingleColorWave());
    stateMachine.addState(new MultiColorWave());
    stateMachine.addState(new CircleColorState());
    stateMachine.addState(new CircleToCenterState());
    stateMachine.addState(new CircleToOutState());
    stateMachine.addState(new ScaleCircleState());
    stateMachine.addState(new VineLineBranchingState());
    stateMachine.addState(new GalaxyOfStarState());
    stateMachine.addState(new BellState());
    stateMachine.addState(new NoiseState());
    stateMachine.addState(new WindowState());
    xml.loadFile("setting.xml");
    startHour = xml.getValue("start:hour", 0);
    startMin = xml.getValue("start:minutes", 0);
    endHour = xml.getValue("end:hour", 0);
    endMin = xml.getValue("end:minutes", 0);
    if ( endHour < 12) endHour += 24;
    cout << startHour << startMin << endl;
    ofAddListener(stateMachine.getSharedData().event.farmEvent, this, &testApp::eventListener);
    if ((ofGetHours() == endHour && ofGetMinutes() < endMin )
            || ( ofGetHours() == startHour && ofGetMinutes() > startMin )
            || ( ofGetHours() > startHour && ofGetHours() < endHour))
    
    {
        sharedData->curState = "NoiseState";
        stateMachine.changeState("NoiseState");
    }
    else
    {
        sharedData->curState = "TextureDevState";
        stateMachine.changeState("TextureDevState");
    }
        
    
    map<string, shared_ptr< itg::ofxState<SharedData> > > states = stateMachine.getStates();
    map<string, shared_ptr< itg::ofxState<SharedData> > >::iterator itr = states.begin();
    while (itr != states.end()) {
        string key = itr->first;
        stateMachine.getSharedData().sendStates(key);
        itr++;
    }
    stateMachine.disableAppEvents();
    statusManager.init(&stateMachine.getSharedData());
}

//--------------------------------------------------------------
void testApp::update(){
    if (!((ofGetHours() == endHour && ofGetMinutes() < endMin )
        || ( ofGetHours() == startHour && ofGetMinutes() > startMin )
        || ( ofGetHours() > startHour && ofGetHours() < endHour)))
        
    {
        {
            if ( sharedData->curState != "TextureDevState")
            {
                sharedData->changeState("TextureDevState");
            }
        }
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    sharedData->update();
    if ( curStateStr != sharedData->curState )
    {
        cout << sharedData->curState << endl;
        curStateStr = sharedData->curState;
    }
    stateMachine.currentState->update();
//    sharedData->sendDmx();
//    statusManager.update();
}
//--------------------------------------------------------------
void testApp::draw(){

    {
        glEnable(GL_DEPTH_TEST);
        stateMachine.currentState->draw();
    }
    ofDisableBlendMode();
    ofEnableAlphaBlending();
    if ( sharedData->showTex )
    {
        if (sharedData->bDefaultBlend)
            glBlendFunc(GL_ONE, GL_NONE);
        glDisable(GL_DEPTH_TEST);
        ofDisableLighting();
        ofSetHexColor(0);
        ofRect(0, 0, stateMachine.getSharedData().tex->getWidth(), stateMachine.getSharedData().tex->getHeight());
        ofSetColor(255,255,255,255);
        stateMachine.getSharedData().tex->draw(ofPoint());
        ofNoFill();
        ofRect(0, 0, SVG_WIDTH, SVG_HEIGHT);
        ofFill();
    }
    if ( sharedData->show2D )
    {
        glDisable(GL_DEPTH_TEST);
        ofDisableLighting();
        ofSetHexColor(0xffffff);
        stateMachine.getSharedData().location.draw2dLine();
        ofSetHexColor(0xffffff);
        ofNoFill();
        ofRect(0, 0, SVG_WIDTH, SVG_HEIGHT);
        ofFill();
    }
}

void testApp::eventListener( FarmEventData& data)
{
    cout << "event receive" << endl;
    if ( data.eventName == "showState")
    {
        cout << "change state " << data.nextState << endl;
        stateMachine.changeState(sharedData->dt.nextState);
    }
    if ( data.eventName == "changeState")
    {
        stateMachine.currentState->hide(data.nextState);
//        stateMachine.changeState(data.nextState);
    }
    if ( data.eventName == "showCur")
    {
        stateMachine.currentState->show();
    }
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
            sharedData->changeState("NoiseState");
            sharedData->doNoise = 2;
            sharedData->noiseAlpha = 1;
            break;
            
        default:
            break;
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