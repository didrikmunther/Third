//
//  CSprite.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSprite.h"

CSprite::CSprite(CSpriteSheet* spriteSheet, Box rect) :
_spriteSheet(spriteSheet), _rect(rect)
{
    _sprite.setTexture(*spriteSheet->getTexture());
    _sprite.setTextureRect(sf::IntRect{rect.x, rect.y, rect.w, rect.h});
}