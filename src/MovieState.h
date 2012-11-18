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
        if ( player.loadMovie("test.mov") )
        {
            cout << "load succeed" << endl;
        }
    }
    
    void stateEnter()
    {
        BaseState::stateEnter();
        player.play();
    }
    
    void update()
    {
        BaseState::update();
        player.update();
    }
  
    void draw()
    {
//        fbo->begin();
//        ofClear(0);
        ofSetHexColor(0xffffff);
        player.draw(0, 0);
//        BaseState::draw();
//        fbo->end();
    }
    
    void stateExit()
    {
        player.stop();
    }
    
    string getName()
    {
        return "MovieState";
    }
};
#endif
