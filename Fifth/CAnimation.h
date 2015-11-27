//
//  CAnimation.h
//  Fifth
//
//  Created by Didrik Munther on 08/07/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CAnimation__
#define __Fifth__CAnimation__

#include <stdio.h>
#include <vector>

#include "CSprite.h"


class CAnimation : public CSprite {

public:
    CAnimation(std::vector<std::string> sprites, int imagesPerSecond);
    
    CSpriteSheet* getSpriteSheet();
    
    void onRender(Box destination, bool flip, int angle, int alpha, CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
private:
    std::vector<std::string> sprites;
    
    int _imagesPerSecond;
    Uint32 startTime;
    int currentFrame;
    
};

#endif /* defined(__Fifth__CAnimation__) */
