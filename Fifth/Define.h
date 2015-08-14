//
//  Define.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//d

#include <stdio.h>
#include <SFML/Graphics.hpp>

#ifndef __Third__Define__
#define __Third__Define__

const int SCREEN_WIDTH          = 1920;
const int SCREEN_HEIGHT         = 1080;
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
    EXIT = sf::Keyboard::Escape,
    
    LEFT = sf::Keyboard::A,
    RIGHT = sf::Keyboard::D,
    DOWN = sf::Keyboard::S,
    UP = sf::Keyboard::W,
    SNEAK = sf::Keyboard::LShift,
    
    BLOCK = sf::Keyboard::L,
    PARTICLEEM = sf::Keyboard::I,
    RESET = sf::Keyboard::J,
    
    TOGGLE_NOCLIP = sf::Keyboard::Num1,
    TOGGLE_HIDDEN = sf::Keyboard::Num2,
    TOGGLE_COLLISION_BOUNDS = sf::Keyboard::Num3,
    
    NEW_WINDOW = sf::Keyboard::Num4,
    LOAD_ASSETS = sf::Keyboard::Num5,
    NEW_CHAT_BUBBLE = sf::Keyboard::Num6,
    
    TARGET_PLAYER = sf::Keyboard::Comma,
    TARGET_BLOCK = sf::Keyboard::Period,
    
    CHANGE_CAMERA_SWAY_UP = sf::Keyboard::Num0,
    CHANGE_CAMERA_SWAY_DOWN = sf::Keyboard::Num9
    
};

enum RenderFlags {
    CLEAR = 0,
    COLLISION_BORDERS = 1 << 0
};

#endif


