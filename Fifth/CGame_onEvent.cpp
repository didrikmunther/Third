//
//  CGame_onEvent.cpp
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"
#include "CGlobalSettings.h"
#include "CBackground.h"

#include "EEnemy.h"
#include "ELiving.h"
#include "EMovable.h"
#include "EMovable.h"
#include "EUtility.h"

#include "NFile.h"
#include "NMouse.h"


void CGame::_onEvent(SDL_Event* event) {
    
    //if(event->key.repeat != 0) return;
    
    EMovable* movable = instance.player->getComponent<EMovable>();
    
    switch(event->type) {
        case SDL_QUIT:
            _isRunning = false;
            break;
            
        case SDL_WINDOWEVENT:
            switch(event->window.event) {
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    isFocused = true;
                    break;
                    
                case SDL_WINDOWEVENT_FOCUS_LOST:
                    isFocused = false;
                    break;
            }
            break;
            
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym) {
                    
                case keyMap::EXIT:
                    _isRunning = false;
                    break;
                    
                case SDLK_8:
                {
                    float angle = 0;
                    int particles = 90;
                    
                    for(int i = 0; i < particles; i++) {
                        angle += (360.0f / particles) / (360 / (2 * M_PI)); // convert raidans to degrees
                        
                        instance.player->shoot(angle, BasicUtilities::DAMAGE);
                    }
                }
                    break;
                    
                case SDLK_7:
                    instance.camera.addCameraShake(100);
                    break;
                    
                case SDLK_m:
                    instance.entityManager.renderFlags ^= RenderFlags::COLLISION_AREA;
                    break;
                    
                case SDLK_n:
                    instance.entityManager.renderFlags ^= RenderFlags::COLLISION_GRID;
                    break;
                    
                case SDLK_b:
                    instance.entityManager.renderFlags ^= RenderFlags::ENTITY_GRID;
                    break;
                    
                case SDLK_v:
                    instance.entityManager.renderFlags ^= RenderFlags::ENEMY_TRIANGLE;
                    break;
                    
                case SDLK_RIGHTBRACKET:
                    CGlobalSettings::GRAVITY += 0.1;
                    
                    if(-0.1 * 100 < CGlobalSettings::GRAVITY * 100 && CGlobalSettings::GRAVITY * 100 < 0.1 * 100)
                        CGlobalSettings::GRAVITY = 0.0f;
                    break;
                    
                case SDLK_LEFTBRACKET:
                    CGlobalSettings::GRAVITY -= 0.1;
                    if(-0.1 * 100 < CGlobalSettings::GRAVITY * 100 && CGlobalSettings::GRAVITY * 100 < 0.1 * 100)
                        CGlobalSettings::GRAVITY = 0.0f;
                    break;
                    
                case keyMap::SNEAK:
                    if(movable)
                        movable->setMovementState(MovementState::SNEAKING_MOVEMENT);
                    break;
                    
                case SDLK_LCTRL:
                    if(movable)
                        movable->setMovementState(MovementState::RUNNING_MOVEMENT);
                    break;
                    
                case keyMap::BLOCK:
                {
                    CEntity* temp = new CEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 40, 40}, SDL_Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                    instance.entityManager.addEntity(temp);
                }
                    break;
                    
                case SDLK_j:
                {
                    auto tempEnemy = new CEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 60, 164}, "player");
                    tempEnemy->addComponent<EEnemy>();
                    tempEnemy->getComponent<EEnemy>()->setTarget(instance.player);
                    tempEnemy->addComponent<ELiving>();
                    tempEnemy->addComponent<EMovable>();
                    tempEnemy->getComponent<EMovable>()->setMovementState(MovementState::SNEAKING_MOVEMENT);
                    instance.entityManager.addEntity(tempEnemy);
                }
                    break;
                    
                case keyMap::TOGGLE_NOCLIP:
                    if(movable)
                        movable->toggleNoclip();
                    break;
                case keyMap::LOAD_ASSETS:
                {
                    //NFile::loadMap("resources/map/testMap1.map", &instance);
                    CBackground* background = new CBackground("bg2", 0.1, BackgroundOffset{0, -450, 10.0f});
                    instance.entityManager.addBackground("main", background);
                }
                    break;
                case keyMap::TOGGLE_HIDDEN:
                    instance.player->toggleProperty(EntityProperty::HIDDEN);
                    break;
                case keyMap::TOGGLE_COLLISION_BOUNDS:
                    //player->toggleProperty(EntityProperty::FLYING);
                    instance.entityManager.toggleRenderFlag(RenderFlags::COLLISION_BORDERS);
                    break;
                case keyMap::NEW_WINDOW:
                {
                    if(instance.window.newWindow(_intro, 640, 480)) {
                        NFile::log(LogType::ERROR, "Window.onInit failed: ", SDL_GetError());
                    }
                    instance.camera.onInit(&instance.window);
                    //NFile::loadMap("resources/map/testMap1.map", &instance);
                }
                    break;
                    
                case keyMap::NEW_CHAT_BUBBLE:
                {
                    const char alphanum[] =                     // Randomize a string
                    "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "          ";
                    
                    std::string text = "";
                    for(int i = 0; i < 100; i++) {
                        text += alphanum[rand() % (sizeof(alphanum) - 1)];
                    }
                    instance.player->say(text, "TESTFONT", ChatBubbleType::SAY);
                    
                    text = "";
                    for(int i = 0; i < 50; i++) {
                        text += alphanum[rand() % (sizeof(alphanum) - 1)];
                    }
                    //instance.entityManager.getEntity("n:bush")->say(text, "TESTFONT", ChatBubbleType::YELL);
                }
                    break;
                    
                case keyMap::TARGET_PLAYER:
                    instance.camera.setTarget(instance.player);
                    break;
                case keyMap::TARGET_BLOCK:
                    instance.camera.setTarget(instance.entityManager.getEntity("m:yrl"));
                    break;
                    
                case keyMap::CHANGE_CAMERA_SWAY_UP:
                    instance.camera.cameraSway += 10;
                    break;
                case keyMap::CHANGE_CAMERA_SWAY_DOWN:
                    if(instance.camera.cameraSway <= 10)
                        instance.camera.cameraSway = 1;
                    else
                        instance.camera.cameraSway -= 10;
                    break;
                    
                default:
                    break;
                    
            }
            break;
            
        case SDL_KEYUP:
            switch(event->key.keysym.sym) {
                case keyMap::SNEAK:
                case SDLK_LCTRL:
                    if(movable)
                        movable->setMovementState(MovementState::WALKING_MOVEMENT);
                    break;
                    
                default:
                    break;
            }
            break;
        default:
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            
            if(NMouse::leftMouseButtonPressed()) {
                auto tempTarget = instance.entityManager.getEntityAtCoordinate(NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera));
                if(tempTarget != nullptr) {
                    std::string toSay = "Name: \"" + instance.entityManager.getNameOfEntity(tempTarget) +
                    "\", CollisionLayer: " + std::to_string(tempTarget->collisionLayer);
                    tempTarget->say(toSay, "TESTFONT", ChatBubbleType::SAY);
                }
            }
            
            if(NMouse::rightMouseButtonPressed()) {
                auto tempTarget = instance.entityManager.getEntityAtCoordinate(NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera));
                if(tempTarget != nullptr) {
                    tempTarget->_isDead = true;
                }
            }
            break;
    }
    
}