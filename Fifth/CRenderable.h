//
//  CRenderable.h
//  Fifth
//
//  Created by Didrik Munther on 06/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CRenderable__
#define __Fifth__CRenderable__

#include <stdio.h>
#include <string>

class CRenderable {

public:
    CRenderable(std::string shaderKey = "") : spriteFollowsCollisionBox(true), _shaderKey(shaderKey), _transparency(255) {};
    
    std::string getShaderKey() { return _shaderKey; }
    void setShaderKey(std::string shaderKey) { _shaderKey = shaderKey; }
    
    void setTransparency(int value) { _transparency = value; }
    int getTransparency() { return _transparency; }
    
    bool spriteFollowsCollisionBox;
    
private:
    std::string _shaderKey;
    int _transparency;
    
};

#endif /* defined(__Fifth__CRenderable__) */
