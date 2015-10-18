//
//  CGame.h
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CGame__
#define __Third__CGame__

#include <stdio.h>
#include <sstream>
#include <string>
#include <SDL2/SDL.h>

#include "CInstance.h"
#include "CChatBubble.h"
#include "CNetwork.h"


class LuaScript;

class CGame {
    friend class CNetwork;
    
public:
    CGame();
    
    int onExecute();
    
private:
    
    // Main functions
    int _onInit();
    void _initRelativePaths();
    void _initLua();
    
    void _onEvent(SDL_Event* event);
    void _handleKeyStates();
    void _onLoop();
    void _onRender();
    int _onCleanup();
    
    // Main variables
    bool _isRunning;
    SDL_Event event;
    CInstance instance;
    
    // Timer variables
    float _lastTime;
    float _timer;
    const float _ns;
    float _delta;
    int _frames;
    int _updates;
    
    // Misc variables
    std::stringstream _title;
    std::string _intro;
    bool isFocused;
    
};

#endif /* defined(__Third__CGame__) */
