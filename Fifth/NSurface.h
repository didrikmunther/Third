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
#include <SDL2/SDL.h>


class CWindow;
class CText;
class CSprite;

namespace NSurface {

    void renderRect(int x, int y, int w, int h, CWindow* window, int r, int g, int b, int a = 255);
    void renderSprite(int x, int y, int w, int h, CSprite* sprite, CWindow* window, SDL_RendererFlip flip, int a = 255);
    void renderText(int x, int y, CText* text, CWindow* window);
    void renderTexture(int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture *texture);
    
};


#endif /* defined(__Third__NSurface__) */
