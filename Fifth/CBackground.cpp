//
//  CBackground.cpp
//  Fifth
//
//  Created by Didrik Munther on 08/07/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CBackground.h"
#include "CAssetManager.h"
#include "CCamera.h"
#include "NSurface.h"

#include <iostream>


CBackground::CBackground(std::string spriteKey, float parallax, BackgroundOffset backgroundOffset) :
    _spriteKey(spriteKey), _parallax(parallax), _backgroundOffset(backgroundOffset) {  }

void CBackground::onRender(CWindow* window, CCamera* camera) {
    CSprite* sprite = CAssetManager::getSprite(_spriteKey);
    
    if(!sprite) return;
    
    int amountX = ceil((sprite->getSource()->w * _backgroundOffset.scale) / camera->getWidth()) + 1;
    
    for(int i = -1; i < amountX; i++) {
        NSurface::renderSprite(i * _backgroundOffset.scale * sprite->getSource()->w + -camera->offsetX() * _parallax, -camera->offsetY() * _parallax + _backgroundOffset.y, sprite->getSource()->w * _backgroundOffset.scale, sprite->getSource()->h * _backgroundOffset.scale, sprite, window, SDL_RendererFlip::SDL_FLIP_NONE);
    }
}

float CBackground::getParallax() {
    return _parallax;
}

void CBackground::setParallax(float parallax) {
    _parallax = parallax;
}