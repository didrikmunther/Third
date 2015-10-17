//
//  CGame.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <fstream>
#include <cmath>
#include <iomanip>

#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Define.h"
#include "CGame.h"
#include "CEntity.h"
#include "CText.h"
#include "CSpriteContainer.h"
#include "CBackground.h"
#include "CGlobalSettings.h"

#include "NMouse.h"
#include "NSurface.h"
#include "NFile.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#include "ResourcePath.hpp"
#endif


CGame::CGame() :
_intro("Third"),
_lastTime(SDL_GetTicks()), _timer(SDL_GetTicks()), _isRunning(true),
_ns(1000.0f / GAME_INTERVAL), _delta(0), _frames(0), _updates(0), isFocused(true) {
}

int CGame::onExecute() {
    
    switch(_onInit()){
        case -1:
            NFile::log(LogType::ERROR, "Initializing failed!");
            _isRunning = false;
        case 0:
            NFile::log(LogType::SUCCESS, "Initializing succesful!");
    }
    
    NFile::log(LogType::ALERT, "Starting game...");
    
    while(_isRunning) {
        while(SDL_PollEvent(&event)){
            _onEvent(&event);
        }
        
        float now = SDL_GetTicks();
        _delta += (now - _lastTime) / _ns;
        _lastTime = now;
        
        while(_delta >= 1) {    // Todo implement variable time step instead of this laggy thing
            if(_delta > 20) {       // To make sure it doesn't freeze
                instance.entityManager.particleCleanup();
            }
            
            _handleKeyStates();
            _onLoop();
            
            _updates++;
            _delta--;
            
            auto sayer = instance.entityManager.getEntity("n:bush");
            if(sayer)
                sayer->say(_title.str() + " Gravity: " + std::to_string(CGlobalSettings::GRAVITY), "TESTFONT", ChatBubbleType::INSTANT_TALK);
        }
        
        _onRender();
        
        _frames++;
        
        if(SDL_GetTicks() - _timer > 1000) {
            _timer += 1000;
            _title.str("");
            _title << _updates << " ups, " << _frames << " fps";
            _updates = 0;
            _frames = 0;
        }
    }
    
    NFile::log(LogType::ALERT, "Ending game...");
    
    return _onCleanup();
    
}

int CGame::_onInit() {
    
    _initRelativePaths();
    NFile::clearFile(LOG_FILE);     // Clear log file
    
    NFile::log(LogType::ALERT, "Initializing game...");
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        NFile::log(LogType::ERROR, "SDL_Init failed: ", SDL_GetError());
        return -1;
    }
    
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        NFile::log(LogType::ERROR, "IMG_Init failed: ", SDL_GetError());
        return -1;
    }
    
    if(TTF_Init() != 0) {
        NFile::log(LogType::ERROR, "TTF_Init failed: ", SDL_GetError());
        return -1;
    }
    
    srand((Uint16)time(nullptr));
    
    if(instance.window.onInit(_intro, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        NFile::log(LogType::ERROR, "Window initialization failed!");
        return -1;
    }
    instance.camera.onInit(&instance.window);
    
    _initLua();
    
    NFile::loadMap("resources/map/testMap1.map", &instance);
    
    CBackground* background = new CBackground("bg2", 0.1, BackgroundOffset{0, -450, 10.0f});
    instance.entityManager.addBackground("main", background);
    
    movableScript = new LuaScript(instance.L, "resources/scripts/Standard/Movable.lua");
    
    auto temp = new CEntity(Box{50, -500, 80, 140}, "playerPink");
    temp->spriteFollowsCollisionBox = false;
    temp->spriteStateTypes[SpriteStateTypes::ASCENDING] =
    temp->spriteStateTypes[SpriteStateTypes::DESCENDING] = "playerPinkRunning";
    temp->addComponent(movableScript);
    instance.entityManager.addEntity(temp);
    instance.player = temp;
    instance.camera.setTarget(temp);
//
    temp = new CEntity(Box{100, -1000, 30 * 5, 28 * 5}, "bush");
    temp->collisionLayer = CollisionLayers::LAYER4;
    instance.entityManager.addEntity(temp, "n:bush");
//
    temp = new CEntity(Box{0, 50, 5000, 20}, SDL_Color{255, 0, 0, 255});
    temp->collisionLayer = -129; // all layers
    temp->addProperty(EntityProperty::STATIC);
    instance.entityManager.addEntity(temp);
    
    temp = new CEntity(Box{0, -4950, 20, 5000}, SDL_Color{255, 0, 0, 255});
    temp->collisionLayer = -129;
    temp->addProperty(EntityProperty::STATIC);
    instance.entityManager.addEntity(temp);    
    
    /*
    
     
     
     "entities": [
     {"name":"m:player2", "type":6, "rect":[300, 0, 80, 140], "spriteContainerKey":"playerPink", "spriteStateTypes":{"1":"playerPinkRunning", "2":"playerPinkRunning"}, "spriteFollowsCollisionBox":false},
     {"name":"m:seeker1", "type":7, "rect":[150,	0, 80, 140], "spriteContainerKey":"playerPink", "target":"m:player2", "spriteStateTypes":{"1":"playerPinkRunning", "2":"playerPinkRunning"}, "spriteFollowsCollisionBox":false},
     {"name":"m:player3", "type":7, "rect":[450, 0, 80, 140], "spriteContainerKey":"playerPink", "spriteStateTypes":{"1":"playerPinkRunning", "2":"playerPinkRunning"}, "spriteFollowsCollisionBox":false},
     {"name":"m:player4", "type":7, "rect":[600, 0, 80, 140], "spriteContainerKey":"playerPink", "spriteStateTypes":{"1":"playerPinkRunning", "2":"playerPinkRunning"}, "spriteFollowsCollisionBox":false},
     {"name":"l:tree",				"rect":[276, -1000, 92, 236], 			"spriteContainerKey":"tree", 			"collisionLayers":4},
     {"name":"n:bush",				"rect":[200, -1000, 120, 108], 			"spriteContainerKey":"bush", 			"collisionLayers":8},
     {								"rect":[0, 465, 5000, 30], 			"colors":[255, 0, 0, 0],				"collisionLayers":-129},
     {								"rect":[0, -20, 30, 500], 			"colors":[255, 0, 0, 0],				"collisionLayers":15}
     ]
     
    */
    
    /*
     LAYER0 // 1
     LAYER1 // 2
     LAYER2 // 4
     LAYER3 // 8
     LAYER4 // 16
     LAYER5 // 32
     LAYER6 // 64
     LAYER7 // 128
    */
    
    return 0;
}

void CGame::_initRelativePaths() {
    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
    #ifdef __APPLE__
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
        {
            // error!
        }
        CFRelease(resourcesURL);
        
        chdir(path);
        NFile::log(LogType::ALERT, "Current Path: ", path);
    #endif
    // ----------------------------------------------------------------------------
}

void CGame::_initLua() {
    luaL_openlibs(instance.L);
    luaL_dofile(instance.L, "resources/scripts/Standard/class.lua");
    luaL_dofile(instance.L, "resources/scripts/Standard/bitwise.lua");
    luaL_dofile(instance.L, "resources/scripts/Standard/standard.lua");
    
    luabridge::getGlobalNamespace(instance.L)
    
        .beginClass<CEntity>("Entity")      // Entity
            .addConstructor<void(*) (Box, SDL_Color)>()
            .addConstructor<void(*) (Box, std::string)>()
            .addData("body", &CEntity::body)
            .addData("collisionSides", &CEntity::collisionSides)
            .addData("properties", &CEntity::properties)
        .endClass()
    
        .beginClass<CBody>("Body")          // Body
            .addConstructor<void(*) (Box)>()
            .addData("velX", &CBody::velX)
            .addData("velY", &CBody::velY)
        .endClass()
    
        .beginClass<CollisionSides>("CollisionSides")
            .addConstructor<void(*) (void)>()
            .addData("top", &CollisionSides::top)
            .addData("bottom", &CollisionSides::bottom)
            .addData("right", &CollisionSides::right)
            .addData("left", &CollisionSides::left)
        .endClass();
}

void CGame::_onLoop() {
    instance.entityManager.onLoop(&instance);
    instance.camera.onLoop();
}

void CGame::_onRender() {
    
    SDL_SetRenderDrawColor(instance.window.getRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(instance.window.getRenderer());
    
    instance.entityManager.onRender(&instance.window, &instance.camera);
    
    SDL_RenderPresent(instance.window.getRenderer());
}

int CGame::_onCleanup() {
    instance.entityManager.onCleanup();
    CAssetManager::onCleanup();
    instance.window.onCleanup();
    
    NFile::log(LogType::ALERT, "Exiting game.");
    
    return 0;
}