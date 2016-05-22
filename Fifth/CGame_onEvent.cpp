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
    
    if(instance.controller)
        instance.controller->onKeyStates(&instance, keystate);
    if(ignoreEvents) return;
    if(instance.player)
        instance.player->onKeyStates(&instance, keystate);
}

void CGame::_onEvent(SDL_Event* event) {
    
    //if(event->key.repeat != 0) return;
    
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
            
        case SDL_TEXTINPUT:
            if(instance.controller)
                instance.controller->onTextInput(&instance, (std::string)event->text.text);
            if(ignoreEvents) break;
            if(instance.player)
                instance.player->onTextInput(&instance, (std::string)event->text.text);
            break;
            
        case SDL_KEYDOWN:
            if(instance.controller)
                instance.controller->onEvent(&instance, event->key.keysym.sym, true);
            if(ignoreEvents) break;
            if(instance.player)
                instance.player->onEvent(&instance, event->key.keysym.sym, true);
            
            switch(event->key.keysym.sym) {
                    
                case SDLK_ESCAPE:
                    _isRunning = false;
                    break;
                    
//                case SDLK_4:
//                {
//                    if(instance.window.newWindow(_intro, 640, 480)) {
//                        NFile::log(LogType::ERROR, "Window.onInit failed: ", SDL_GetError());
//                    }
//                    instance.camera->onInit(&instance.window);
//                    instance.loadAssets("resources/map/testMap1.assets");
//                    restart();
//                }
//                    break;
                    
                default:
                    break;
                    
            }
            break;
            
        case SDL_KEYUP:
            if(instance.controller)
                instance.controller->onEvent(&instance, event->key.keysym.sym, false);
            if(ignoreEvents) break;
            if(instance.player)
                instance.player->onEvent(&instance, event->key.keysym.sym, false);
            break;
            
        default:
            break;
            
        case SDL_MOUSEBUTTONDOWN:
        {
            int x = NMouse::relativeMouseX(instance.camera);
            int y = NMouse::relativeMouseY(instance.camera);
            auto entities = instance.entityManager.getEntitiesAtCoordinate(x, y);
            
//            if(entities.size() > 0) {
//                (*(entities.end() - 1))->onClick(x, y, &instance);
//            }
            
            for(auto& i: entities) {
                i->onClick(x, y, &instance);
            }
            
        }
            break;
    }
    
}