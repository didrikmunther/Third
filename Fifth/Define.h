//
//  Define.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//d

#include <stdio.h>
#include <SDL2/SDL.h>


#ifndef __Third__Define__
#define __Third__Define__

const int SCREEN_WIDTH          = 1280;
const int SCREEN_HEIGHT         = 720;
const int SCREEN_BPP            = 32;

const int GAME_INTERVAL          = 60;

const char LOG_FILE[]           = "debug.log";

struct Box {
    int x, y;
    int w, h;
    
    bool operator==(const Box& a) {
        return  x == a.x &&
                y == a.y &&
                w == a.w &&
                h == a.h;
    }
};

struct Area {
    int w, h;
};

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

enum RenderFlags {
    CLEAR = 0,
    COLLISION_BORDERS   = 1 << 0,
    COLLISION_AREA      = 1 << 1,
    COLLISION_GRID      = 1 << 2,
    COLLISION_FULL_AREA = 1 << 3,
    ENTITY_GRID         = 1 << 4,
    ENEMY_TRIANGLE      = 1 << 5
};

#endif


