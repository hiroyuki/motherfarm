//
//  Header.h
//  motherfarmLED
//
//  Created by 堀 宏行 on 11/18/12.
//
//

#ifndef motherfarmLED_TreeBeam_h
#define motherfarmLED_TreeBeam_h
#define TREE_INDEX 1
#define TREE_AMOUNT 16
#define BEAM_INDEX 58
#define BEAM_AMOUNT 9
#define THIS_X 280
#define THIS_Y 250
class TreeBeam
{
public:
    string ipAddress;
    unsigned char* data;
    ofVec2f pos;
    ofVec3f vec3;
    ofPoint treePos;
    vector< ofPoint > beamPoss;
    void setup()
    {
        ipAddress = "192.168.11.118";
        data = new unsigned char[BEAM_INDEX + BEAM_AMOUNT];
        for ( int i = 0; i < BEAM_INDEX + BEAM_AMOUNT; i++)
        {
            data[ i ] = 0;
        }
    }
    
    void update()
    {
        
    }
    
    void debugDraw()
    {
        ofSetColor(255, 0, 0);
        float radius = 0.5f;
        for( int i = 0; i < 16; i++ )
        {
            float posx = radius * cos((float)i * PI / 15.f);
            float posy = radius * sin((float)i * PI / 15.f);
            ofCircle(vec3.x + posx, vec3.y + posy, vec3.z, 0.1);
        }
        radius = 1.0f;
        ofSetHexColor(0xffffff);
        for( int i = 0; i < 3; i++ )
        {
            float posx = radius * cos((float)(i + 0.5) * PI / 3.f);
            float posy = radius * sin((float)(i + 0.5) * PI / 3.f);
            ofCircle(vec3.x + posx, vec3.y + posy, vec3.z, 0.1);
        }
    }
    
    void sendDmx()
    {
        
    }
    
    string getIpAddress()
    {
        return ipAddress;
    }
    
    int getSize()
    {
        return BEAM_INDEX + BEAM_AMOUNT;
    }
    
    virtual void checkPos(ofxGrabCam &cam)
    {
        pos = ofVec2f(THIS_X, THIS_Y);
        vec3 = cam.getPosition3f(pos.x * SVG_SCALE + SVG_OFFSET_X,
                                          pos.y * SVG_SCALE + SVG_OFFSET_Y);
    }
};


#endif
