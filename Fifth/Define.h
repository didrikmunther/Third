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

const int GAMEINTERVAL          = 60;
const int DESPAWN_HEIGHT        = 10000;
const float GRAVITY             = 0.3;

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

enum MovementState {
    WALKING_MOVEMENT = 0,
    SNEAKING_MOVEMENT,
    RUNNING_MOVEMENT
};

enum ValueTypes {
    HEALTH = 0,
    KEVLAR,
    STAMINA,
    ENERGY,
    VALUETYPES_TOTAL
};

enum StatTypes {
    ARMOUR = 0,
    ATTACK_POWER,
    ATTACK_SPEED,
    STATTYPES_TOTAL
};

struct UtilityPosition {
    int x, y;
};

enum BasicUtilities {
    DAMAGE = 0,
    HEAL,
    NONE
};

enum EntityProperty {
    COLLIDABLE  = 1 << 0,
    FLYING      = 1 << 1,
    HIDDEN      = 1 << 2,
    STATIC      = 1 << 3,
    FLIP        = 1 << 4,
    FLIP_FREEZED= 1 << 5
};

struct CollisionSides {
    bool collisionTop,
    collisionBottom,
    collisionRight,
    collisionLeft;
};

enum CollisionLayers { // CEntity
    LAYER0      = 1 << 0,   // 1
    LAYER1      = 1 << 1,   // 2
    LAYER2      = 1 << 2,   // 4
    LAYER3      = 1 << 3,   // 8
    LAYER4      = 1 << 4,   // 16
    LAYER5      = 1 << 5,   // 32
    LAYER6      = 1 << 6,   // 64
    LAYER7      = 1 << 7,   // 128
};

enum EntityTypes { // CEntity
    Entity = 0,
    Particle,   // 1
    Movable,    // 2
    UtilityParticle, // 3
    Living,     // 4
    Npc,        // 5
    Player,     // 6
    Enemy       // 7
};

enum SpriteStateTypes { // CEntity
    IDLE = 0,
    ASCENDING,
    DESCENDING,
    WALKING,
    SNEAKING,
    RUNNING,
    DUCKING,
    TOTAL_SPRITESTATETYPES
};

enum ChatBubbleType { // CChatBubble
    SAY = 0,
    YELL = 1,
    WHISPER = 2,
    INSTANT_TALK = 3
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
    ENTITY_GRID         = 1 << 4
};

#endif


