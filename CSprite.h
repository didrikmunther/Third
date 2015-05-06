
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
#include <SFML/Graphics.hpp>

class CSprite : public CRenderable {
    
public:
    CSprite(CSpriteSheet* spriteSheet, sf::IntRect rect, std::string shaderKey = "");
    
    sf::IntRect* getOffset();
    sf::Sprite* getSprite();
    
private:
    sf::IntRect _rect;
    sf::Sprite _sprite;
    
};

#endif /* defined(__Third__CSprite__) */
