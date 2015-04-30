//
//  CSprite.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSprite.h"

CSprite::CSprite(CSpriteSheet* spriteSheet, sf::IntRect rect)
{
    sprite.setTexture(*spriteSheet->getTexture());
    sprite.setTextureRect(rect);
}

sf::Sprite* CSprite::getSprite() {
    return &sprite;
}

void CSprite::setPosition(int x, int y) {
    sprite.setPosition(x, y);
}