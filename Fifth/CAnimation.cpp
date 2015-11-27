//
//  CAnimation.cpp
//  Fifth
//
//  Created by Didrik Munther on 08/07/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "CAnimation.h"
#include "CAssetManager.h"
#include "CSpriteSheet.h"
#include "CWindow.h"
#include "CCamera.h"

#include "NSurface.h"


CAnimation::CAnimation(std::vector<std::string> sprites, int imagesPerSecond)
    : sprites(sprites)
    , CSprite(nullptr, Box{0,0,0,0})
    , startTime(SDL_GetTicks())
    , _imagesPerSecond(imagesPerSecond)
    , currentFrame(0)
{
}

CSpriteSheet* CAnimation::getSpriteSheet() {
    for(auto& i: sprites) {
        auto sprite = CAssetManager::getSprite(i);
        if(!sprite)
            continue;
        
        auto sheet = sprite->getSpriteSheet();
        if(!sheet)
            continue;
        
        return sheet;
    }
    
    return nullptr;
}

void CAnimation::onRender(Box destination, bool flip, int angle, CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    int timePassed = SDL_GetTicks() - startTime;
    if(timePassed > _imagesPerSecond * 1000) {
        startTime = SDL_GetTicks();
        currentFrame++;
        
        if(currentFrame >= sprites.size())
            currentFrame = 0;
    }
    
    auto sprite = CAssetManager::getSprite(sprites[currentFrame]);
    if(!sprite)
        return;
    
    NSurface::renderTexture(destination, *sprite->getSource(), window->getRenderer(), getSpriteSheet()->getTexture(), flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE, angle);
}