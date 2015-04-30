//
//  CText.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CText.h"
#include "NSurface.h"

CText::CText(std::string text, int size, std::string fontKey, CAssetManager* assetManager, sf::Color color) :
    text(text), size(size), fontKey(fontKey), assetManager(assetManager), color(color) {
}

void CText::onRender(int x, int y, sf::RenderWindow* window, CCamera* camera) {
    if(assetManager->getFont(fontKey) != nullptr)
        NSurface::renderText(x - camera->offsetX(), y - camera->offsetY(), this, window);
}

void CText::onRender(int x, int y, sf::RenderWindow* window) {
    if(assetManager->getFont(fontKey) != nullptr)
        NSurface::renderText(x, y, this, window);
}

sf::Font* CText::getFont() {
    return assetManager->getFont(fontKey);
}

int CText::getSize() {
    return size;
}

std::string* CText::getText() {
    return &text;
}

sf::Color* CText::getColor() {
    return &color;
}