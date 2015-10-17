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

#include "NFile.h"
#include "NMouse.h"

#include "CEntity.h"


void CGame::_handleKeyStates() {
    
    if(!isFocused)
        return;

    const Uint8* keystate = SDL_GetKeyboardState(NULL);
//
//    // Movement
//    
//    EMovable* movable = nullptr;
//    if(instance.player)
//        movable = instance.player->getComponent<EMovable>();
//    else
//        return;

    auto movable = instance.player->getComponent("Standard/Movable");
    
    if(keystate[SDL_SCANCODE_D]) {
        movable->callSimpleFunction("goRight");
    }
    if(keystate[SDL_SCANCODE_A]) {
        movable->callSimpleFunction("goLeft");
    }
    
    if(keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_SPACE]) {
        movable->callSimpleFunction("goUp");
    }
    
    if(keystate[SDL_SCANCODE_S]) {
        movable->callSimpleFunction("goDown");
    }
//
//    // Other
//    
//    if(NMouse::leftMouseButtonPressed()) { // damage particle
//        int mousePosX = NMouse::relativeMouseX(&instance.camera) - instance.player->body->getX();
//        int mousePosY = NMouse::relativeMouseY(&instance.camera) - (instance.player->body->getY() - 100);
//        float angle = atan2(mousePosY, mousePosX);
//        
//        instance.player->shoot(angle, BasicUtilities::DAMAGE);
//    }
//    
//    if(NMouse::rightMouseButtonPressed()) {   // heal particle
//        int mousePosX = NMouse::relativeMouseX(&instance.camera) - instance.player->body->getX();
//        int mousePosY = NMouse::relativeMouseY(&instance.camera) - (instance.player->body->getY() - 100);
//        float angle = atan2(mousePosY, mousePosX);
//        
//        instance.player->shoot(angle, BasicUtilities::HEAL);
//    }
}

void CGame::_onEvent(SDL_Event* event) {
    
    //if(event->key.repeat != 0) return;
    
//    EMovable* movable = nullptr;
//    if(instance.player)
//        movable = instance.player->getComponent<EMovable>();
    
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
                    
                case SDLK_ESCAPE:
                    _isRunning = false;
                    break;
                    
                case SDLK_8:
                {
                    float angle = 0;
                    int particles = 90;
                    
                    for(int i = 0; i < particles; i++) {
                        angle += (360.0f / particles) / (360 / (2 * M_PI)); // convert raidans to degrees
                        
//                        instance.player->shoot(angle, BasicUtilities::DAMAGE);
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
                    
                case SDLK_LSHIFT:
//                    if(movable)
//                        movable->setMovementState(MovementState::SNEAKING_MOVEMENT);
                    break;
                    
                case SDLK_LCTRL:
//                    if(movable)
//                        movable->setMovementState(MovementState::RUNNING_MOVEMENT);
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
//                    auto tempEnemy = new CEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 60, 164}, "player");
//                    tempEnemy->addComponent<EEnemy>();
//                    tempEnemy->getComponent<EEnemy>()->setTarget(instance.player);
//                    tempEnemy->addComponent<ELiving>();
//                    tempEnemy->addComponent<EMovable>();
//                    tempEnemy->getComponent<EMovable>()->setMovementState(MovementState::SNEAKING_MOVEMENT);
//                    instance.entityManager.addEntity(tempEnemy);
                }
                    break;
                    
                case SDLK_1:
                {
                    auto movable = instance.player->getComponent("Standard/Movable");
                    
                    movable->callSimpleFunction("toggleNoClip");
                }
                    break;
                case SDLK_5:
                {
                    //NFile::loadMap("resources/map/testMap1.map", &instance);
                    CBackground* background = new CBackground("bg2", 0.1, BackgroundOffset{0, -450, 10.0f});
                    instance.entityManager.addBackground("main", background);
                }
                    break;
                case SDLK_2:
                    instance.player->toggleProperty(EntityProperty::HIDDEN);
                    break;
                case SDLK_3:
                    //player->toggleProperty(EntityProperty::FLYING);
                    instance.entityManager.toggleRenderFlag(RenderFlags::COLLISION_BORDERS);
                    break;
                case SDLK_4:
                {
                    if(instance.window.newWindow(_intro, 640, 480)) {
                        NFile::log(LogType::ERROR, "Window.onInit failed: ", SDL_GetError());
                    }
                    instance.camera.onInit(&instance.window);
                    //NFile::loadMap("resources/map/testMap1.map", &instance);
                }
                    break;
                    
                case SDLK_6:
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
            switch(event->key.keysym.sym) {
                case SDLK_LSHIFT:
                case SDLK_LCTRL:
//                    if(movable)
//                        movable->setMovementState(MovementState::WALKING_MOVEMENT);
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
                    tempTarget->isDead = true;
                }
            }
            break;
    }
    
}