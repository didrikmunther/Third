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
#include <SFML/Graphics.hpp>

class CRenderable {

public:
    CRenderable(std::string shaderKey = "") : _shaderKey("SHADER1") {};
    
    std::string getShaderKey() { return _shaderKey; }
    void setShaderKey(std::string shaderKey) { _shaderKey = shaderKey; }
    
private:
    std::string _shaderKey;
    
};

#endif /* defined(__Fifth__CRenderable__) */
