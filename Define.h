//
//  Define.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//d

#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
#define SCREEN_BPP     32

#define GAMEINTERVAL    60
#define DESPAWN_HEIGHT  10000
#define GRAVITY         0.5

const int SCREEN_FLAGS = SDL_WINDOW_SHOWN; // | SDL_WINDOW_FULLSCREEN;
const int RENDERER_FLAGS = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_SOFTWARE;

enum keyMap {
    EXIT = SDLK_ESCAPE,
    
    LEFT = SDLK_a,
    RIGHT = SDLK_d,
    DOWN = SDLK_s,
    UP = SDLK_w,
    SNEAK = SDLK_LSHIFT,
    
    BLOCK = SDLK_l,
    PARTICLEEM = SDLK_i,
    RESET = SDLK_j,
    
    TOGGLE_NOCLIP = SDLK_1,
    TOGGLE_HIDDEN = SDLK_2,
    TOGGLE_COLLISION_BOUNDS = SDLK_3,
    
    NEW_WINDOW = SDLK_4,
    LOAD_ASSETS = SDLK_5,
    NEW_CHAT_BUBBLE = SDLK_6,
    
    TARGET_PLAYER = SDLK_COMMA,
    TARGET_BLOCK = SDLK_PERIOD,
    
    CHANGE_CAMERA_SWAY_UP = SDLK_0,
    CHANGE_CAMERA_SWAY_DOWN = SDLK_9
    
};

enum EntityProperty {
    COLLIDABLE  = 1 << 0,
    FLYING      = 1 << 1,
    HIDDEN      = 1 << 2,
    STATIC      = 1 << 3
};

enum RenderFlags {
    COLLISION_BORDERS = 1 << 0
};

enum ChatBubbleType {
    SAY = 0,
    YELL = 1,
    WHISPER = 2
};




