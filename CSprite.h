
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
#include <SFML/Graphics.hpp>

class CSprite {
    
public:
    CSprite(CSpriteSheet* spriteSheet, sf::IntRect rect);
    
    sf::Sprite* getSprite();
    sf::IntRect* getOffset();
    
private:
    sf::IntRect rect;
    sf::Sprite sprite;
    
};

#endif /* defined(__Third__CSprite__) */
