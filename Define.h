//
//  Define.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#define GAMEINTERVAL 60

const int SCREEN_FLAGS = SDL_WINDOW_SHOWN;

enum keyMap {
    EXIT = SDLK_ESCAPE,
    
    LEFT = SDLK_a,
    RIGHT = SDLK_d,
    DOWN = SDLK_s,
    UP = SDLK_w,
    
    BLOCK = SDLK_l,
    PARTICLEEM = SDLK_i,
    PARTICLE = SDLK_k,
    RESET = SDLK_j
    
};