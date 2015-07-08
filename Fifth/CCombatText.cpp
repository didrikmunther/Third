//
//  CCombatText.cpp
//  Fifth
//
//  Created by Didrik Munther on 02/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CCombatText.h"
#include "NSurface.h"

CCombatText::CCombatText(int x, int y, sf::Color color, int size, std::string text, std::string fontKey) : CGuiText(x, y, text, fontKey) {
    
    _livingTime = 2;
    
    int variator = 100;
    int randomPosX = rand() % variator - variator / 2;
    int randomPosY = rand() % variator - variator / 2;
    _x += randomPosX;
    _y += randomPosY;
    
    _textObj = new CText(text, size, fontKey, color);
}

void CCombatText::onLoop() {
    if(_clock.getElapsedTime().asMilliseconds() > _creationTime + _livingTime * 1000)
        toRemove = true;
    _y--;
}

void CCombatText::onRender(CWindow* window, CCamera* camera) {    
    _textObj->onRender(_x, _y, *window->getRenderTexture(), camera);
}