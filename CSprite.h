
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
#include <SDL2/SDL.h>
#include "CSpriteSheet.h"

class CSprite {
    
public:
    CSprite(CSpriteSheet* spriteSheet, SDL_Rect source);
    
    CSpriteSheet* getSpriteSheet();
    virtual SDL_Rect* getSource();       // For later letting animation class inherit from this class
    
    
private:
    CSpriteSheet* spriteSheet;
    SDL_Rect source;
    
};

#endif /* defined(__Third__CSprite__) */
