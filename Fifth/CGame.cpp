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
#include "CCombatText.h"

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
    
    restart();
    
    return 0;
}

void CGame::restart() {
    instance.entityManager.onCleanup();
    
    
    CBackground* background = new CBackground("bg2", 0.1, BackgroundOffset{0, -450, 10.0f});
    instance.entityManager.addBackground("main", background);

    auto movable = CAssetManager::addLuaScript(instance.L, "resources/scripts/Standard/Movable.lua");
    auto npc = CAssetManager::addLuaScript(instance.L, "resources/scripts/Standard/Npc.lua");
    auto living = CAssetManager::addLuaScript(instance.L, "resources/scripts/Standard/Living.lua");
    CAssetManager::addLuaScript(instance.L, "resources/scripts/Standard/Bullet.lua");
    
    auto temp = new CEntity(Box{50, -500, 80, 140}, "playerPink");
    temp->spriteFollowsCollisionBox = false;
    temp->spriteStateTypes[SpriteStateTypes::ASCENDING] =
    temp->spriteStateTypes[SpriteStateTypes::DESCENDING] = "playerPinkRunning";
    temp->addComponent(movable);
    temp->addComponent(living);
    instance.entityManager.addEntity(temp);
    instance.player = temp;
    instance.camera.setTarget(temp);
    
    for(int i = 0; i < 50; i++) {
        temp = new CEntity(Box{i * 6 + 32, -600, 5, 5}, Color{255, 255, 0});
        temp->addComponent(movable);
        temp->addComponent(npc);
        temp->getComponent("Standard/Npc")->object.beginCall("setTarget");
        luabridge::Stack<CEntity*>::push(instance.L, instance.player);
        temp->getComponent("Standard/Npc")->object.endCall(1, 0);
        temp->collisionLayer = CollisionLayers::LAYER6;
        instance.entityManager.addEntity(temp);
    }
    
    temp = new CEntity(Box{100, -1000, 30 * 5, 28 * 5}, "bush");
    temp->collisionLayer = CollisionLayers::LAYER4;
    instance.entityManager.addEntity(temp, "n:bush");
    
    temp = new CEntity(Box{0, 50, 5000, 20}, Color{255, 0, 0, 255});
    temp->collisionLayer = -129; // all layers
    temp->addProperty(EntityProperty::STATIC);
    instance.entityManager.addEntity(temp);
    
    temp = new CEntity(Box{0, -4950, 20, 5000}, Color{255, 0, 0, 255});
    temp->collisionLayer = -129;
    temp->addProperty(EntityProperty::STATIC);
    instance.entityManager.addEntity(temp);
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
    
        .beginNamespace("game")
            .addFunction("getScript", &CAssetManager::getLuaScript)
            .addFunction("getTime", &CGame::getTime)
        .endNamespace()
    
        .beginClass<CLuaScript>("LuaScript")
            .addFunction("getName", &CLuaScript::getName)
        .endClass()
    
        .beginClass<CEntity>("Entity")      // Entity doesn't need a constructor, because you should use the entityManager.create.. functions
            .addCFunction("getComponent", &CEntity::getComponent)
            .addFunction("addComponent", &CEntity::addComponent)
            .addData("entityManager", &CEntity::entityManager)
            .addData("body", &CEntity::body)
            .addData("isDead", &CEntity::isDead)
            .addData("toRemove", &CEntity::toRemove)
            .addData("collisionSides", &CEntity::collisionSides)
            .addData("properties", &CEntity::properties)
            .addFunction("hasProperty", &CEntity::hasProperty)
            .addData("transparency", &CEntity::transparency)
            .addFunction("say", (void (CEntity::*)(std::string, std::string, int)) &CEntity::say)
            .addFunction("addTextObject", &CEntity::addTextObject)
            .addFunction("addCombatText", &CEntity::addCombatText)
            .addFunction("compare", &CEntity::compare)
        .endClass()
    
        .beginClass<CComponent>("Component")
            .addFunction("renderRect", &CComponent::renderRect)
            .addFunction("renderLine", &CComponent::renderLine)
        .endClass()
    
        .beginClass<Box>("Box")
            .addConstructor<void(*) (int, int, int, int)>()
            .addData("x", &Box::x)
            .addData("y", &Box::y)
            .addData("w", &Box::w)
            .addData("h", &Box::h)
        .endClass()
    
        .beginClass<Color>("Color")
            .addConstructor<void(*) (int, int, int, int)>()
        .endClass()
    
        .beginClass<CGuiText>("GuiText")
            .addConstructor<void(*) (int, int, std::string, std::string)>()
        .endClass()
    
        .beginClass<CCombatText>("CombatText")
            .addConstructor<void(*) (int, int, Color, int, std::string, std::string)>()
        .endClass()
    
        .beginClass<CBody>("Body")
            .addConstructor<void(*) (Box)>()
            .addData("velX", &CBody::velX)
            .addData("velY", &CBody::velY)
            .addData("box", &CBody::_rect)
        .endClass()
    
        .beginClass<Line>("Line")
            .addConstructor<void(*) (int, int, int, int, Color)>()
        .endClass()
    
        .beginClass<Triangle>("Triangle")
            .addConstructor<void(*) (Line, Line, Line)>()
        .endClass()
    
        .beginClass<CEntityManager>("EntityManager")
            .addFunction("addEntity", &CEntityManager::addEntity)
            .addFunction("createColoredEntity", (CEntity* (CEntityManager::*)(Box, Color)) &CEntityManager::createEntity)
            .addFunction("createSpriteEntity", (CEntity* (CEntityManager::*)(Box, std::string)) &CEntityManager::createEntity)
        .endClass()
    
        .beginClass<CollisionSides>("CollisionSides")
            .addConstructor<void(*) (void)>()
            .addData("top", &CollisionSides::top)
            .addData("bottom", &CollisionSides::bottom)
            .addData("right", &CollisionSides::right)
            .addData("left", &CollisionSides::left)
        .endClass();
}

int CGame::getTime() {
    return SDL_GetTicks();
}

void CGame::_onLoop() {
    instance.entityManager.onLoop(&instance);
    instance.camera.onLoop();
}

void CGame::_onRender() {
    SDL_SetRenderDrawColor(instance.window.getRenderer(), 250, 250, 250, 255);
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