//
//  CSprite.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSprite.h"

CSprite::CSprite(CSpriteSheet* spriteSheet, SDL_Rect source) :
    spriteSheet(spriteSheet), source(source) {
    
}

CSpriteSheet* CSprite::getSpriteSheet() {
    return spriteSheet;
}

SDL_Rect* CSprite::getSource() {
    return &source;
}