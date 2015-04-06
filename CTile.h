//
//  CTile.h
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CTile__
#define __Third__CTile__

#include <stdio.h>
#include <SDL2/SDL.h>

class CTile {
    
public:
    CTile(SDL_Rect rect, int type);
    
    void onRender(SDL_Renderer* renderer);
    
private:
    int type;
    SDL_Rect rect;
    
};

#endif /* defined(__Third__CTile__) */
