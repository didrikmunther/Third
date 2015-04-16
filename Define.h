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
#define DESPAWN_HEIGHT 10000
#define GRAVITY 0.5
#define CAMERA_SWAY 10

const int SCREEN_FLAGS = SDL_WINDOW_SHOWN; // | SDL_WINDOW_FULLSCREEN;
const int RENDERER_FLAGS = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;

enum keyMap {
    EXIT = SDLK_ESCAPE,
    
    LEFT = SDLK_a,
    RIGHT = SDLK_d,
    DOWN = SDLK_s,
    UP = SDLK_w,
    
    BLOCK = SDLK_l,
    PARTICLEEM = SDLK_i,
    RESET = SDLK_j,
    
    TOGGLE_NOCLIP = SDLK_1,
    TOGGLE_HIDDEN = SDLK_2,
    TOGGLE_FLYING = SDLK_3,
    TOGGLE_STATIC = SDLK_4,
    TOGGLE_COLLIDE = SDLK_5,
    
    TARGET_PLAYER = SDLK_COMMA,
    TARGET_BLOCK = SDLK_PERIOD
    
};

enum EntityProperty {
    COLLIDABLE  = 0x1,
    FLYING      = 0x2,
    HIDDEN      = 0x4,
    STATIC      = 0x8
};

