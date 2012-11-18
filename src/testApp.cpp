#include "testApp.h"
int startHour, startMin, endHour, endMin;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(40);
    sharedData = new SharedData();
    sharedData->setup();
//    stateMachine.addState(new BaseState());
    states.push_back(new ParseState(sharedData));
    states.push_back(new TextureDevState(sharedData));
    states.push_back(new SingleColorWave(sharedData));
    states.push_back(new MultiColorWave(sharedData));
    states.push_back(new CircleColorState(sharedData));
    states.push_back(new CircleToCenterState(sharedData));
    states.push_back(new CircleToOutState(sharedData));
    states.push_back(new ScaleCircleState(sharedData));
    states.push_back(new VineLineBranchingState(sharedData));
    states.push_back(new GalaxyOfStarState((sharedData)));
    states.push_back(new BellState(sharedData));
    states.push_back(new NoiseState(sharedData));
    states.push_back(new WindowState(sharedData));

    xml.loadFile("setting.xml");
    startHour = xml.getValue("start:hour", 0);
    startMin = xml.getValue("start:minutes", 0);
    endHour = xml.getValue("end:hour", 0);
    endMin = xml.getValue("end:minutes", 0);
    ofAddListener(sharedData->event.farmEvent, this, &testApp::eventListener);
//    if ((ofGetHours() == endHour && ofGetMinutes() < endMin )
//            || ( ofGetHours() == startHour && ofGetMinutes() > startMin )
//            || ( ofGetHours() > startHour && ofGetHours() < endHour))
    
//    {
//        sharedData->curState = "NoiseState";
//        stateMachine.changeState("NoiseState");
//    }
//    else
//    {
//        sharedData->curState = "TextureDevState";
//        stateMachine.changeState("TextureDevState");
//    }
    
    

    statusManager.init(sharedData);
}

//--------------------------------------------------------------
void testApp::update(){
//    if (!((ofGetHours() == endHour && ofGetMinutes() < endMin )
//        || ( ofGetHours() == startHour && ofGetMinutes() > startMin )
//        || ( ofGetHours() > startHour && ofGetHours() < endHour)))
//        
//    {
//        {
//            if ( sharedData->curState != "TextureDevState")
//            {
//                sharedData->changeState("TextureDevState");
//            }
//        }
//    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    sharedData->update();
    if ( curStateStr != sharedData->curState )
    {
        cout << sharedData->curState << endl;
        curStateStr = sharedData->curState;
    }
    stateMachine.currentState->update();
//
//    fbo.readToPixels(*colorPixels);
//    tex->loadData(colorPixels->getPixels(), SVG_WIDTH, SVG_HEIGHT, GL_RGBA);
    sharedData->sendDmx();
//    statusManager.update();
}
//--------------------------------------------------------------
void testApp::draw(){
    {
        glEnable(GL_DEPTH_TEST);
        stateMachine.currentState->draw();
        if ( sharedData->showParse )
        {
            ofEnableLighting();
            sharedData->location.debugDraw();
        }
        sharedData->location.drawLed();
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