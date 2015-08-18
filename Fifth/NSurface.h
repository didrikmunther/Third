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
#include "CSprite.h"
#include "CText.h"
#include "CWindow.h"

namespace NSurface {

    void renderRect(int x, int y, int w, int h, CWindow* window, int r, int g, int b, int a = 255);
    void renderRect(SDL_Rect* rect, CWindow* window, int r, int g, int b, int a = 255);
    
    void renderEntity(CEntity* entity, CWindow* window, int x, int y);
    void renderSprite(CSprite* sprite, CWindow* window, int x, int y);
    void renderText(int x, int y, CText* text, CWindow* window);
    void renderTexture(SDL_Renderer* renderer, SDL_Rect destination, SDL_Texture *texture);
    
};


#endif /* defined(__Third__NSurface__) */
