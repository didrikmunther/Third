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
#include "CWindow.h"

namespace NSurface {

    void renderRect(int x, int y, int w, int h, sf::RenderTarget& target, int r, int g, int b);
    void renderRect(sf::IntRect rect, sf::RenderTarget& target, int r, int g, int b);
    
    void renderEntity(CEntity* entity, CWindow* window, sf::IntRect destination, int properties = 0);
    void renderText(int x, int y, CText* textObj, sf::RenderTarget& target);
    
};


#endif /* defined(__Third__NSurface__) */
