//
//  CSprite.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSprite.h"

CSprite::CSprite(CSpriteSheet* spriteSheet, sf::IntRect rect) : rect(rect)
{
    sprite.setTexture(*spriteSheet->getTexture());
    sprite.setTextureRect(rect);
}

sf::Sprite* CSprite::getSprite() {
    return &sprite;
}

sf::IntRect* CSprite::getOffset() {
    return &rect;
}