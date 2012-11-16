#include "testApp.h"
//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(40);
    sharedData = &stateMachine.getSharedData();
    sharedData->setup();
    stateMachine.addState(new BaseState());
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
    
    ofAddListener(stateMachine.getSharedData().event.farmEvent, this, &testApp::eventListener);
    stateMachine.getSharedData().changeState("GalaxyOfStarState");
    
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
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    sharedData->update();
    stateMachine.currentState->update();
    sharedData->sendDmx();
    statusManager.update();
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
    if ( data.eventName == "changeState")
    {
        stateMachine.changeState(data.nextState);
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