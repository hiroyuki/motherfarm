//
//  ParseState.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/4/12.
//
//

#ifndef motherfarmLED_ParseState_h
#define motherfarmLED_ParseState_h
#include "BaseState.h"

class MovieState : public BaseState
{
public:
    ofVideoPlayer player;
    MovieState(SharedData *sharedData):BaseState(sharedData){
        gui.addToggle("MovieState", isActive);
    }
    
    void setup()
    {
        BaseState::setup();
        if ( player.loadMovie("B_full_compose_3min40s_1120a.mov") )
        {
            player.setVolume(0);
            cout << "load succeed" << endl;
        }
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
        player.setPosition(0);
        player.play();
    }
    
    void update()
    {
        BaseState::update();
        player.update();
//        cout << player.getCurrentFrame() << endl;
//        if ( player.getCurrentFrame() > 1280 && sharedData->location.treeModel->mode_tree != TREE_BPM500
//            && player.getCurrentFrame() < 4700)
//        {
//            sharedData->location.treeModel->mode_tree = TREE_BPM500;
//        }
//        else if ( player.getCurrentFrame() >  4700)
//        {
//            sharedData->location.treeModel->mode_tree = TREE_BPM2000;
//        }
    }
  
    void draw()
    {
//        fbo->begin();
//        ofClear(0);
        ofEnableAlphaBlending();
        ofSetColor(255, 255, 255, sharedData->curveA*255.f);
        player.draw(0, 0);
        ofSetColor(255,255,255);
//        BaseState::draw();
//        fbo->end();
    }
    
    void stateExit()
    {
        BaseState::stateExit();
        player.stop();
    }
    
    string getName()
    {
        return "MovieState";
    }
};
#endif
