
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

#include "Define.h"


class CSpriteSheet;

class CSprite {
    
public:
    CSprite(CSpriteSheet* spriteSheet, Box rect);
    
    CSpriteSheet* getSpriteSheet() { return _spriteSheet; }
    virtual SDL_Rect* getSource() { return &_source; }
    
private:
    CSpriteSheet* _spriteSheet; // Don't modify from here
    SDL_Rect _source;
    
};

#endif /* defined(__Third__CSprite__) */
