//
//  CGame.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"
#include <iostream>
#include "NSurface.h"
#include <cmath>
#include "Define.h"
#include "NMouse.h"
#include <SDL2_image/SDL_image.h>

CGame::CGame() :
running(true), intro("Physics"),
//WIDTH(640), HEIGHT(480), BPP(32), camera(WIDTH, HEIGHT),
lastTime(SDL_GetTicks()), timer(SDL_GetTicks()),
ns(1000.0f / (float)GAMEINTERVAL), delta(0), frames(0), updates(0) {
}

CGame::~CGame() {
}

int CGame::onExecute() {
    
    std::cout << "Initializing game...\n";
    
    switch(onInit()){
        case -1:
            std::cout << "Initializing failed!\n";
            return 1;
            break;
        case 0:
            std::cout << "Initializing succesful!\n";
            break;
    }
    
    std::cout << "Starting game...\n";
    
    while(running) {
        while(SDL_PollEvent(&event)){
            onEvent(&event);
        }
        
        //std::cout << "CameraX: " << camera.offsetX() << ", CameraY: " << camera.offsetY() << " \n";
        
        float now = SDL_GetTicks();
        delta += (now - lastTime) / ns;
        lastTime = now;
        
        while(delta >= 1) {
            //std::cout << (int)floor(delta) << " | ";
            if(delta > 20) {       // To make sure it doesn't freeze
                entityManager.particleCleanup();
            }
            
            handleKeyStates();
            onLoop();
            
            updates++;
            delta--;
        }
        
        onRender();
        
        frames++;
        
        if(SDL_GetTicks() - timer > 1000) {
            timer += 1000;
            title.str("");
            title << intro << " | " << updates << " ups, " << frames << " fps";
            window.setTitle(title.str());
            updates = 0;
            frames = 0;
        }
        
        //SDL_Delay(7);
        
    }
    
    std::cout << "Ending game...\n";
    
    return onCleanup();
    
}

int CGame::onInit() {
    
    srand((Uint16)time(nullptr));
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        puts("SDL_Init error");
        return -1;
    }
    
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        puts("IMG_Init error");
        return -1;
    }
    
    if(window.onInit(intro, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_FLAGS, RENDERER_FLAGS))
        return -1;
    camera.onInit(&window);
    
    assetManager.addSpriteSheet("MAIN", "resources/gfx.png", window.getRenderer());                 // All these are temporary for testing
    //assetManager.addSprite("player", "MAIN", SDL_Rect{11,5,43,53});                     // Will have a system that loads from text file
    assetManager.addSprite("player", "MAIN", SDL_Rect{90,220,18,30});
    assetManager.addSprite("bush", "MAIN", SDL_Rect{160, 91, 30, 28});
    assetManager.addSprite("tree", "MAIN", SDL_Rect{7,64,23,59});
    assetManager.addSpriteSheet("BG", "resources/bg.png", window.getRenderer());
    assetManager.addSprite("background", "BG", SDL_Rect{0,0,128,64});
    
//    auto bg = entityManager.addEntity(SDL_Rect{0,0,1000, 1000}, "background", &assetManager);
//    bg->removeProperty(EntityProperty::COLLIDABLE);
//    bg->addProperty(EntityProperty::STATIC);
    
    player = new CPlayer(SDL_Rect{30, 30, 18 * 3, 30 * 3}, "player", &assetManager);
    entityManager.addEntity(player, "m:player");                                                // Layer system: z -> a. visible to nonvisible
    camera.setTarget(player);
    
    entityManager.addEntity(SDL_Rect{0 - 30 / 2, 480 - 30 / 2, 5000, 30}, SDL_Color{255, 0, 0, 0});
    entityManager.addEntity(SDL_Rect{0 - 30 / 2, 480 - 500, 30, 500}, SDL_Color{255, 0, 0, 0});
    auto tree = entityManager.addEntity(SDL_Rect{276, 229, 23 * 4, 59 * 4}, "tree", &assetManager, "l:tree");
    tree->removeProperty(EntityProperty::COLLIDABLE);
    tree->addProperty(EntityProperty::STATIC);
    block = entityManager.addEntity(SDL_Rect{200, 357, 60 * 2, 54 * 2}, "bush", &assetManager, "n:bush");
    block->removeProperty(EntityProperty::COLLIDABLE);
    block->addProperty(EntityProperty::STATIC);

    return 0;
}

void CGame::handleKeyStates() {
    
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    if(keystate[SDL_SCANCODE_D]) {
        player->goRight();
    }
    if(keystate[SDL_SCANCODE_A]) {
        player->goLeft();
    }
    
    if(keystate[SDL_SCANCODE_W]) {
        player->goUp();
    }
    
    if(keystate[SDL_SCANCODE_S]) {
        player->goDown();
    }
}

void CGame::onEvent(SDL_Event* event) {
    
    
    
    //if(event->key.repeat != 0) return;
    
    switch(event->type) {
        case SDL_QUIT:
            running = false;
            break;
            
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym) {
                    
                case keyMap::EXIT:
                    running = false;
                    break;
                    
                case keyMap::SNEAK:
                    player->isSneaking = true;
                    break;
                    
                case keyMap::BLOCK:
                {
                    CEntity* temp = entityManager.addEntity(SDL_Rect{NMouse::relativeMouseX(&camera) - 30 / 2, NMouse::relativeMouseY(&camera) - 30 / 2, 40, 40}, SDL_Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                }
                    break;
                    
                case keyMap::PARTICLEEM:
                    entityManager.addParticleEmitter(SDL_Rect{NMouse::relativeMouseX(&camera) - 4 / 2, NMouse::relativeMouseY(&camera) - 4 / 2, 10, 10}, SDL_Color{ (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0}, 20, 2, 4, 6, 0.3);
                    break;
                    
                case keyMap::RESET:
                    *player = CPlayer(SDL_Rect{30, 30, 30, 30}, SDL_Color{255, 255, 0, 255});
                    break;
                    
                case keyMap::TOGGLE_NOCLIP:
                    player->toggleProperty(EntityProperty::COLLIDABLE);
                    player->toggleProperty(EntityProperty::FLYING);
                    break;
                case keyMap::TOGGLE_COLLIDE:
                    //player->toggleProperty(EntityProperty::COLLIDABLE);
                    
                    assetManager.addSpriteSheet("MAIN", "resources/gfx.png", window.getRenderer());                 // All these are temporary for testing
                    //assetManager.addSprite("player", "MAIN", SDL_Rect{11,5,43,53});                     // Will have a system that loads from text file
                    assetManager.addSprite("player", "MAIN", SDL_Rect{90,220,18,30});
                    assetManager.addSprite("bush", "MAIN", SDL_Rect{160, 91, 30, 28});
                    assetManager.addSprite("tree", "MAIN", SDL_Rect{7,64,23,59});
                    assetManager.addSpriteSheet("BG", "resources/bg.png", window.getRenderer());
                    assetManager.addSprite("background", "BG", SDL_Rect{0,0,128,64});
                    break;
                case keyMap::TOGGLE_HIDDEN:
                    player->toggleProperty(EntityProperty::HIDDEN);
                    break;
                case keyMap::TOGGLE_FLYING:
                    //player->toggleProperty(EntityProperty::FLYING);
                    entityManager.toggleRenderFlag(RenderFlags::COLLISION_BORDERS);
                    break;
                case keyMap::TOGGLE_STATIC:
                {
                    //player->toggleProperty(EntityProperty::STATIC);
                    window.newWindow(intro, 600, 400, SCREEN_FLAGS, RENDERER_FLAGS);
                    camera.onInit(&window);
                    assetManager.onCleanup();
            }
                    break;
                    
                case keyMap::TARGET_PLAYER:
                    camera.setTarget(player);
                    break;
                case keyMap::TARGET_BLOCK:
                    camera.setTarget(block);
                    break;
                    
                case keyMap::CHANGE_CAMERA_SWAY_UP:
                        camera.cameraSway += 10;
                    break;
                case keyMap::CHANGE_CAMERA_SWAY_DOWN:
                    if(camera.cameraSway <= 10)
                        camera.cameraSway = 1;
                    else
                        camera.cameraSway -= 10;
                    break;
                    
            }
            break;
        
        case SDL_KEYUP:
            switch(event->key.keysym.sym) {
                case keyMap::SNEAK:
                    player->isSneaking = false;
                    break;
            }
            break;
    }
    
}

void CGame::onLoop() {
    entityManager.onLoop();
    camera.onLoop();
}

void CGame::onRender() {
    
    SDL_SetRenderDrawColor(window.getRenderer(), 255, 255, 255, 255);
    SDL_RenderClear(window.getRenderer());
    
    entityManager.onRender(window.getRenderer(), &camera);
    
    SDL_RenderPresent(window.getRenderer());
    
}

int CGame::onCleanup() {
    entityManager.onCleanup();
    assetManager.onCleanup();
    window.onCleanup();
    
    IMG_Quit();
    SDL_Quit();
    return 0;
}