//
//  CText.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CText.h"
#include "NSurface.h"

CText::CText(std::string text, int size, std::string fontKey, sf::Color color) :
    _text(text), _size(size), _fontKey(fontKey), _color(color) {
}

void CText::onRender(int x, int y, sf::RenderTarget& renderTarget, CCamera* camera) {
    if(CAssetManager::getFont(_fontKey) != nullptr)
        NSurface::renderText(x - camera->offsetX(), y - camera->offsetY(), this, renderTarget);
}

void CText::onRender(int x, int y, sf::RenderTarget& renderTarget) {
    if(CAssetManager::getFont(_fontKey) != nullptr)
        NSurface::renderText(x, y, this, renderTarget);
}

sf::Font* CText::getFont() {
    return CAssetManager::getFont(_fontKey);
}

int CText::getSize() {
    return _size;
}

std::string* CText::getText() {
    return &_text;
}

sf::Color* CText::getColor() {
    return &_color;
}