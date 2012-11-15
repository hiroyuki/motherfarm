/*
 *  ofxBlurFast.h
 *
 *  Created by Patricio González Vivo on 25/11/11.
 *  Copyright (c) 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 *  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *  OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ************************************************************************************ 
 *  
 *  Fade: control the sharp/unsharp mode
 *
 *  1.0 -> unsharp: smooth or a fast blur
 *  0.0
 * -1.0 -> sharp
 *
 */

#ifndef OFXUNSHARP
#define OFXUNSHARP

#include "ofMain.h"
#include "ofxFXObject.h"

class ofxUnsharp : public ofxFXObject {	
public:
    ofxUnsharp(){
        passes = 3;
        internalFormat = GL_RGBA;
        
        // Fade constant
        value0 = 0.f;   
        
        // In this example the tex0 it´s more like a backbuffer 
        fragmentShader = "#version 120\n \
#extension GL_ARB_texture_rectangle : enable\n \
\n\
float kernel[9];\n\
\n\
uniform sampler2DRect tex0;\n\
uniform float value0;\n\
\n\
vec2 offset[9];\n\
\n\
void main(void){\n\
    vec2  st = gl_TexCoord[0].st;\n\
    vec4 sum = vec4(0.0);\n\
    \n\
    offset[0] = vec2(-1.0, -1.0);\n\
    offset[1] = vec2(0.0, -1.0);\n\
    offset[2] = vec2(1.0, -1.0);\n\
    \n\
    offset[3] = vec2(-1.0, 0.0);\n\
    offset[4] = vec2(0.0, 0.0);\n\
    offset[5] = vec2(1.0, 0.0);\n\
    \n\
    offset[6] = vec2(-1.0, 1.0);\n\
    offset[7] = vec2(0.0, 1.0);\n\
    offset[8] = vec2(1.0, 1.0);\n\
    \n\
    kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;\n\
    kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;\n\
    kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;\n\
    \n\
    int i = 0;\n\
    for (i = 0; i < 4; i++){\n\
        vec4 tmp = texture2DRect(tex0, st + offset[i]);\n\
        sum += tmp * kernel[i];\n\
    }\n\
    \n\
    for (i = 5; i < 9; i++){\n\
        vec4 tmp = texture2DRect(tex0, st + offset[i]);\n\
        sum += tmp * kernel[i];\n\
    }\n\
    \n\
    vec4 color0 = texture2DRect(tex0, st + offset[4]);\n\
    sum += color0 * kernel[4];\n\
    \n\
    gl_FragColor = (1.0 - value0) * color0 +  value0 * vec4(sum.rgb, color0.a);\n\
}\n\
\n";  
    };
    
	void setFade(float _fade) { value0 = _fade;};
    
	void update(){
        pingPong.src->begin();
        textures[0].draw(0,0);
        pingPong.src->end();
        
        for(int i = 0; i < passes; i++) {
            pingPong.dst->begin();
            shader.begin();
            // In this example the tex0 it´s more like a backbuffer 
            shader.setUniformTexture("tex0", pingPong.src->getTextureReference(), 0 );
            shader.setUniform1f("value0", value0);
            renderFrame();
            shader.end();
            pingPong.dst->end();
            
            pingPong.swap();
        }
        
        pingPong.swap();
	}
	
protected:
	float       value0;
};
#endif
