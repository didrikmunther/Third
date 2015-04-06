//
//  CSurface.h
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CSurface__
#define __Third__CSurface__

#include <stdio.h>
#include <SDL2/SDL.h>

namespace CSurface {

    void renderRect(int x, int y, int w, int h, SDL_Renderer *renderer, int r, int g, int b);
    
    void renderRect(SDL_Rect rect, SDL_Renderer *renderer, int r, int g, int b);
    
    
};


#endif /* defined(__Third__CSurface__) */
