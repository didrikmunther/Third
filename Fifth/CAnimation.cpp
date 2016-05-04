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
#include "CEntity.h"

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

void CAnimation::onRender(CEntity* entity, Box destination, bool flip, int angle, int alpha, CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    int timePassed = SDL_GetTicks() - startTime;
    if(timePassed >= (1.0f / (_imagesPerSecond * entity->fpsFactor) * 1000)) {
        startTime = SDL_GetTicks();
        currentFrame++;
        
        if(currentFrame >= sprites.size())
            currentFrame = 0;
        
        _source = *CAssetManager::getSprite(sprites[currentFrame])->getSource();
    }
    
    auto sprite = CAssetManager::getSprite(sprites[currentFrame]);
    if(!sprite) {
        NSurface::renderRect(destination.x, destination.y, destination.w, destination.h, window, 255, 0, 255);
        return;
    }
    
    sprite->onRender(entity, destination, flip, angle, alpha, window, camera, renderFlags);
    
    //NSurface::renderTexture(destination, *sprite->getSource(), window->getRenderer(), getSpriteSheet()->getTexture(), flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE, angle);
}