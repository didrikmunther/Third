//
//  CGame.h
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CGame__
#define __Third__CGame__

#include <SDL2/SDL.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include "CEntityManager.h"
#include "CAssetManager.h"
#include "CWindow.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CChatBubble.h"

class CGame {
    
public:
    
    CGame();
    ~CGame();
    
    int onExecute();
    
private:
    
    // Main functions
    
    int onInit();
    
    void onEvent(SDL_Event* event);
    
    void handleKeyStates();
    
    void onLoop();
    
    void onRender();
    
    int onCleanup();
    
    // Main variables
    
    SDL_Event event;
    
    bool running;
    
    CCamera camera;
    CPlayer* player;
    
    // Timer variables
    
    float lastTime;
    float timer;
    const float ns;
    float delta;
    int frames;
    int updates;
    
    // Misc variables
    
    std::stringstream title;
    std::string intro;
    
    // Game variables
    
    CEntityManager entityManager;
    CAssetManager assetManager;
    CWindow window;
    
    // Temp variables
    
    CEntity* block;
    
};

#endif /* defined(__Third__CGame__) */
