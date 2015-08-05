
//
//  CSprite.h
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CSprite__
#define __Third__CSprite__

#include <stdio.h>
#include "CSpriteSheet.h"
#include "CRenderable.h"
#include "Define.h"
#include <SFML/Graphics.hpp>

class CSprite {
    
public:
    CSprite(CSpriteSheet* spriteSheet, Box rect);
    
    CSpriteSheet* getSpriteSheet() { return _spriteSheet; }
    Box* getOffset() { return &_rect; }
    virtual sf::Sprite* getSprite() { return &_sprite; }
    
private:
    CSpriteSheet* _spriteSheet; // Don't modify from here
    Box _rect;
    sf::Sprite _sprite;
    
};

#endif /* defined(__Third__CSprite__) */
