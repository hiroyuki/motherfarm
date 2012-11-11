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
    ofAddListener(stateMachine.getSharedData().event.farmEvent, this, &testApp::eventListener);
    stateMachine.getSharedData().changeState("ScaleCircleState");
    
    map<string, shared_ptr< itg::ofxState<SharedData> > > states = stateMachine.getStates();
    map<string, shared_ptr< itg::ofxState<SharedData> > >::iterator itr = states.begin();
    while (itr != states.end()) {
        string key = itr->first;
        stateMachine.getSharedData().sendStates(key);
        itr++;
    }
    stateMachine.disableAppEvents();
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    sharedData->update();
    stateMachine.currentState->update();
}
//--------------------------------------------------------------
void testApp::draw(){

    {
        glEnable(GL_DEPTH_TEST);
        stateMachine.currentState->draw();
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
    if ( sharedData->showTex )
    {
        glDisable(GL_DEPTH_TEST);
        ofDisableLighting();
        ofSetHexColor(0xffffff);
        stateMachine.getSharedData().tex->draw(ofPoint());
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