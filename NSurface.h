//
//  NSurface.h
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__NSurface__
#define __Third__NSurface__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "CSprite.h"
#include "CText.h"

namespace NSurface {

    void renderRect(int x, int y, int w, int h, sf::RenderWindow* window, int r, int g, int b);
    void renderRect(sf::IntRect rect, sf::RenderWindow* window, int r, int g, int b);
    void renderSprite(CSprite* sprite, sf::RenderWindow* window, sf::IntRect destination);
//    void renderSprite(CSprite* sprite, sf::RenderWindow* window, sf::IntRect destination, double angle, SDL_Point* center, SDL_RendererFlip flip);
    void renderText(int x, int y, CText* textObj, sf::RenderWindow* window);
    
};


#endif /* defined(__Third__NSurface__) */
