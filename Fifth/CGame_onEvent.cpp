//
//  CGame_onEvent.cpp
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"
#include "CBackground.h"
#include "CAssetManager.h"

#include "NFile.h"
#include "NMouse.h"

#include "CEntity.h"


void CGame::_handleKeyStates() {
    
    if(!isFocused)
        return;

    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    instance.controller->onKeyStates(&instance, keystate);
    if(ignoreEvents) return;
    instance.player->onKeyStates(&instance, keystate);
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
            instance.controller->onEvent(&instance, event->key.keysym.sym, true);
            if(ignoreEvents) break;
            instance.player->onEvent(&instance, event->key.keysym.sym, true);
            
            switch(event->key.keysym.sym) {
                    
                case SDLK_ESCAPE:
                    _isRunning = false;
                    break;
                    
                case SDLK_4:
                {
                    if(instance.window.newWindow(_intro, 640, 480)) {
                        NFile::log(LogType::ERROR, "Window.onInit failed: ", SDL_GetError());
                    }
                    instance.camera->onInit(&instance.window);
                    instance.loadAssets("resources/map/testMap1.assets");
                    restart();
                }
                    break;
                    
                case SDLK_g:
                {
                    rapidjson::Document d;
                    d.Parse("{}");
                
                    rapidjson::Value entityValues(rapidjson::kObjectType);
                    instance.entityManager.onSerialize(&entityValues, &d.GetAllocator(), &instance);
                
                    d.AddMember("this", entityValues, d.GetAllocator());
                
                    rapidjson::StringBuffer sb;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
                    d.Accept(writer);
                    
                    std::cout << sb.GetString() << "\n";
                    quickSave = sb.GetString();
                }
                    break;
                    
                case SDLK_h:
                {
                    rapidjson::Document d;
                    d.Parse(quickSave.c_str());
                    //instance.entityManager.onCleanup();
                    
                    instance.entityManager.onDeserialize(&d["this"], &instance);
                    
                    //instance.player = instance.entityManager.getEntity("5:Player");
                    //instance.controller = instance.entityManager.getEntity("Controller");
                    
                    //instance.camera->setTarget(instance.player);
                }
                    break;
                    
                default:
                    break;
                    
            }
            break;
            
        case SDL_KEYUP:
            instance.controller->onEvent(&instance, event->key.keysym.sym, false);
            if(ignoreEvents) break;
            instance.player->onEvent(&instance, event->key.keysym.sym, false);
            break;
            
        default:
            break;
            
        case SDL_MOUSEBUTTONDOWN:
            
            if(NMouse::leftMouseButtonPressed()) {
                auto tempTarget = instance.entityManager.getEntityAtCoordinate(NMouse::relativeMouseX(instance.camera), NMouse::relativeMouseY(instance.camera));
                if(tempTarget != nullptr) {
                    std::string toSay = "Name: \"" + instance.entityManager.getNameOfEntity(tempTarget) +
                    "\", CollisionLayer: " + std::to_string(tempTarget->collisionLayer);
                    tempTarget->say(toSay, "TESTFONT", ChatBubbleType::SAY);
                }
            }
            
            if(NMouse::rightMouseButtonPressed()) {
                auto tempTarget = instance.entityManager.getEntityAtCoordinate(NMouse::relativeMouseX(instance.camera), NMouse::relativeMouseY(instance.camera));
                if(tempTarget != nullptr) {
                    tempTarget->isDead = true;
                }
            }
            break;
    }
    
}