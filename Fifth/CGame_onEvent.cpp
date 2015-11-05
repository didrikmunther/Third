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
#include "CAssetManager.h"

#include "NFile.h"
#include "NMouse.h"

#include "CEntity.h"


void CGame::_handleKeyStates() {
    
    if(!isFocused)
        return;

    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    instance.entityManager.onKeyStates(&instance, keystate);
}

void CGame::_onEvent(SDL_Event* event) {
    
    //if(event->key.repeat != 0) return;
    
    CComponent* movable = nullptr;
    if(instance.player)
        movable = instance.player->getComponent("Standard/Movable");
    
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
            instance.entityManager.onEvent(&instance, event->key.keysym.sym, true);
            
            switch(event->key.keysym.sym) {
                    
                case SDLK_ESCAPE:
                    _isRunning = false;
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
                    
                case SDLK_c:
                    instance.entityManager.renderFlags ^= RenderFlags::TRACE_BULLETS;
                    break;
                    
                case SDLK_x:
                    instance.entityManager.renderFlags ^= RenderFlags::RENDER_COMBAT_TEXT;
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
                    
                case SDLK_l:
                {
                    CEntity* temp = new CEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 40, 40}, Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                    temp->addCollisionLayer(-129);
                    instance.entityManager.addEntity(temp);
                }
                    break;
                    
                case SDLK_j:
                {
                    auto temp = new CEntity(Box(NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 80, 140),
                                            "playerPink");
                    instance.entityManager.addEntity(temp);
                    temp->spriteFollowsCollisionBox = false;
                    temp->spriteStateTypes[SpriteStateTypes::ASCENDING] =
                    temp->spriteStateTypes[SpriteStateTypes::DESCENDING] = "playerPinkRunning";
                    temp->addComponent(&instance, CAssetManager::getLuaScript("Standard/Living"));
                    temp->addComponent(&instance, CAssetManager::getLuaScript("Standard/Npc"));
                    temp->addComponent(&instance, CAssetManager::getLuaScript("Standard/Movable"));
                    temp->getComponent("Standard/Npc")->onDeserialize("{\"target\":\"" + instance.entityManager.getNameOfEntity(instance.player) + "\"}");
                    temp->getComponent("Standard/Movable")->onDeserialize("{\"walking_movement_speed\":3.0, \"jumpPower\":5.0}");
                }
                    break;
                    
                case SDLK_4:
                {
                    if(instance.window.newWindow(_intro, 640, 480)) {
                        NFile::log(LogType::ERROR, "Window.onInit failed: ", SDL_GetError());
                    }
                    instance.camera.onInit(&instance.window);
                    NFile::loadMap("resources/map/testMap1.map", &instance);
                    restart();
                }
                    break;
                    
                case SDLK_COMMA:
                    instance.camera.setTarget(instance.player);
                    break;
                case SDLK_PERIOD:
                    instance.camera.setTarget(instance.entityManager.getEntity("m:yrl"));
                    break;
                    
                case SDLK_0:
                    instance.camera.cameraSway += 10;
                    break;
                case SDLK_9:
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
            instance.entityManager.onEvent(&instance, event->key.keysym.sym, false);
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
                    tempTarget->isDead = true;
                }
            }
            break;
    }
    
}