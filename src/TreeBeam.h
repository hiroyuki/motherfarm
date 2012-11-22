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
enum TREE_MODE
{
    TREE_FULL, TREE_NOISE, TREE_BPM4000, TREE_BPM2000, TREE_BPM1000, TREE_BPM500, TREE_NONE,
    BEAM_ROUND4000, BEAM_ROUND2000, BEAM_ROUND1000, BEAM_ROUND500
};

class TreeModel
{
private:
    int roundCounter;
    float treeValue;
public:
    int mode_tree;
    int mode_beam;
    
    int beamValue;
    bool bpm4000, bpm2000, bpm1000, bpm500;
    
    unsigned char* beam;
    TreeModel():roundCounter(0), treeValue(0)
    {
        beam = new unsigned char[9];
    }
    
    void update(bool bpm4000, bool bpm2000, bool bpm1000, bool bpm500)
    {
        
        this->bpm4000 = bpm4000;
        this->bpm1000 = bpm1000;
        this->bpm500 = bpm500;
        switch (mode_tree) {
            case TREE_FULL:
                treeValue = 1.f;
                break;
            case TREE_NOISE:
                treeValue = (ofNoise(ofGetElapsedTimeMillis() * 0.001f) * 50.f +10)/255.f;
                break;
            case TREE_BPM500:
                if (bpm500)
                {
                    treeValue = 1;
                }
                else
                {
                    treeValue = ofLerp(treeValue, 0.5, 0.1);
                }
                break;
            case TREE_BPM1000:
                if ( bpm1000 )
                {
                    treeValue = 1;
                }
                else
                {
                    treeValue = ofLerp(treeValue, 0.5, 0.01);
                }
                break;
            case TREE_BPM2000:
                if ( bpm2000 )
                {
                    treeValue = 1;
                }
                else
                {
                    treeValue = ofLerp(treeValue, 0.5, 0.05);
                }
                break;
            case TREE_BPM4000:
                if ( bpm4000 )
                {
                    treeValue = 1;
                }
                else
                {
                    treeValue = ofLerp(treeValue, 0.5, 0.01);
                }
                break;
            default:
//                treeValue = 0;
                break;
        }
        switch (mode_beam) {
            case BEAM_ROUND4000:
                if ( bpm4000) roundCounter++;break;
            case BEAM_ROUND2000:
                if ( bpm2000) roundCounter++;break;
            case BEAM_ROUND1000:
                if ( bpm1000) roundCounter++;break;
            case BEAM_ROUND500:
                if ( bpm500) roundCounter++;break;
                break;
            default:
                break;
        }
    }
    
    int getRoundCounter()
    {
        return roundCounter;
    }
    
    float getTreeValue()
    {
        return treeValue;
    }
    
    float getBeamValue()
    {
        return beamValue;
    }
    
    unsigned char* getRoundBeamValue()
    {
        if ( mode_beam >= BEAM_ROUND4000)
        {
            int count = roundCounter % 3;
            for( int i = 0; i < 9; i++)
            {
                if (count == (int)(i / 3))
                {
                    beam[ i ] = 255;
                }
                else
                {
                    beam[ i ] = 0;
                }
            }
        }
        else if( TREE_FULL )
        {
            for( int i = 0; i < 9; i++)
            {
                beam[ i ] = 255;
            }
        }
        return beam;
    }
};

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
        ipAddress = "192.168.11.106";
        data = new unsigned char[BEAM_INDEX + BEAM_AMOUNT];
        for ( int i = 0; i < BEAM_INDEX + BEAM_AMOUNT; i++)
        {
            if ( i < TREE_AMOUNT)
            {
                data[ i ] = 0;
            }
            else
            {
                data[ i ] = 0;
            }
        }
    }
    
    void update(TreeModel* model, bool bpm4000, bool bpm2000, bool bpm1000, bool bpm500, float alpha)
    {
        model->update(bpm4000, bpm2000, bpm1000, bpm500);
        float treevalue = ofNoise(ofGetElapsedTimeMillis() * 0.001f) * 50.f +10;
        if ( model->mode_beam == BEAM_ROUND4000 ||
                model->mode_beam == BEAM_ROUND2000 ||
                model->mode_beam == BEAM_ROUND1000 ||
                model->mode_beam == BEAM_ROUND500)
        {
            memcpy(data + BEAM_INDEX -1, model->getRoundBeamValue(), BEAM_AMOUNT);
        }
        for ( int i = 0; i < BEAM_INDEX + BEAM_AMOUNT; i++)
        {
            if ( i < TREE_AMOUNT)
            {
                data[ i ] = model->getTreeValue() * 255.f * alpha;
            }
            else
            {
                data[i] = ((int)data[i])*alpha;
            }
        }
    }
    
    void debugDraw()
    {
        ofSetColor(255, 0, 0);
        float radius = 0.5f;
        for( int i = 0; i < 16; i++ )
        {
            float posx = radius * cos((float)i * PI / 15.f);
            float posy = radius * sin((float)i * PI / 15.f);
            int r = (int)data[ i ];
            ofSetColor( r );
            ofCircle(vec3.x + posx, vec3.y + posy, vec3.z, 0.1);
        }
        radius = 1.0f;
        for( int i = 0; i < 3; i++ )
        {
            float posx = radius * cos((float)(i + 0.5) * PI / 3.f);
            float posy = radius * sin((float)(i + 0.5) * PI / 3.f);
            int r, g, b;
            r = (int)data[ BEAM_INDEX + i * 3 ];
            g = (int)data[ BEAM_INDEX + i * 3 + 1];
            b = (int)data[ BEAM_INDEX + i * 3  + 2];
            ofSetColor(r, g, b);
            ofCircle(vec3.x + posx, vec3.y + posy, vec3.z, 0.1);
        }
        ofSetHexColor(0xffffff);
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
