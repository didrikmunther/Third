//
//  Define.cpp
//  Third
//
//  Created by Didrik Munther on 13/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//d

#include <stdio.h>
#include <SFML/Graphics.hpp>

#define NATIVE_SCREEN_WIDTH   1920
#define NATIVE_SCREEN_HEIGHT  1080

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
#define SCREEN_BPP     32

#define GAMEINTERVAL    60
#define DESPAWN_HEIGHT  10000
#define GRAVITY         0.5

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

enum EntityProperty {
    COLLIDABLE  = 1 << 0,
    FLYING      = 1 << 1,
    HIDDEN      = 1 << 2,
    STATIC      = 1 << 3,
    FLIP        = 1 << 4
};

enum RenderFlags {
    COLLISION_BORDERS = 1 << 0
};

enum ChatBubbleType {
    SAY = 0,
    YELL = 1,
    WHISPER = 2
};




