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


enum GameType {
    CLIENT      = 1 << 0,
    SERVER      = 1 << 1,
};

class CLuaScript;
class CGameClient;

class CGame {
public:
    CGame(GameType gameType = GameType::CLIENT);
    
    int onExecute(std::string host);
    void restart();
    
    // Main functions
    int _onInit();
    static void _initLua(lua_State* L, const char* path);
    static int setLuaPath(lua_State* L, const char* path);
    
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
    std::string host;

    void _restart();
    bool toRestart;
    
    static int getTime();
    static int getWidth() { return SCREEN_WIDTH; }
    static int getHeight() { return SCREEN_HEIGHT; }
    
    bool ignoreEvents;
    
    std::string _path;
    std::string quickSave;
    
    GameType gameType;
    static bool globalServerInit;
    
    SDL_mutex* mutex;
    CGameClient* gameClient;
    
};

#endif /* defined(__Third__CGame__) */
