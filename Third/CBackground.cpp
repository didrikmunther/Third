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

void CBackground::onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance) {
    addNumber(value, alloc, "x", _backgroundOffset.x);
    addNumber(value, alloc, "y", _backgroundOffset.y);
    addNumber(value, alloc, "scale", _backgroundOffset.scale);
    
    addString(value, alloc, "spriteKey", _spriteKey);
    addNumber(value, alloc, "parallax", _parallax);
}

void CBackground::onDeserialize(const rapidjson::Value* value, CInstance* instance) {
    assignInt(value, "x", &_backgroundOffset.x);
    assignInt(value, "y", &_backgroundOffset.y);
    assignFloat(value, "scale", &_backgroundOffset.scale);
    
    assignString(value, "spriteKey", &_spriteKey);
    assignFloat(value, "parallax", &_parallax);
}

float CBackground::getParallax() {
    return _parallax;
}

void CBackground::setParallax(float parallax) {
    _parallax = parallax;
}