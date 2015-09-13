//
//  CBackground.cpp
//  Fifth
//
//  Created by Didrik Munther on 08/07/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CBackground.h"
#include "CCamera.h"
#include "CSpriteContainer.h"
#include "NSurface.h"


CBackground::CBackground(std::string spriteContainerKey, float parallax, BackgroundOffset backgroundOffset) :
    _spriteContainerKey(spriteContainerKey), _parallax(parallax), _backgroundOffset(backgroundOffset) {  }

void CBackground::onRender(CWindow* window, CCamera* camera) {
    CSpriteContainer* spriteContainer = CAssetManager::getSpriteContainer(_spriteContainerKey);
    
    if(!spriteContainer) return;
    
    float scale = 9.0f;
    
    NSurface::renderSprite(-camera->offsetX() * _parallax + _backgroundOffset.x, -camera->offsetY() * _parallax + _backgroundOffset.y, spriteContainer->spriteArea.w * scale, spriteContainer->spriteArea.h * scale, spriteContainer->getSprite(), window, SDL_RendererFlip::SDL_FLIP_NONE);
}

float CBackground::getParallax() {
    return _parallax;
}

void CBackground::setParallax(float parallax) {
    _parallax = parallax;
}

std::string CBackground::getSpriteContainerKey() {
    return _spriteContainerKey;
}

void CBackground::setSpriteContainerKey(std::string spriteContainerKey) {
    _spriteContainerKey = spriteContainerKey;
}