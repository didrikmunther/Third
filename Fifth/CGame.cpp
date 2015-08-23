//
//  CGame.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"
#include <iostream>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include "NSurface.h"
#include "NFile.h"
#include <cmath>
#include "Define.h"
#include "NMouse.h"
#include "CText.h"
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#include "ResourcePath.hpp"
#endif
#include "CSpriteContainer.h"
#include <fstream>
#include "EMovable.h"
#include "EUtilityParticle.h"
#include "ENpc.h"
#include "ELiving.h"

CGame::CGame() :
_intro("Physics"),
_lastTime(SDL_GetTicks()), _timer(SDL_GetTicks()), _isRunning(true),
_ns(1000.0f / (float)GAMEINTERVAL), _delta(0), _frames(0), _updates(0), isFocused(true) {
}

CGame::~CGame() {
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
        while(SDL_PollEvent(&_event)){
            _onEvent();
        }
        
        float now = SDL_GetTicks();
        _delta += (now - _lastTime) / _ns;
        _lastTime = now;
        
        while(_delta >= 1) {    // Todo implement variable time step instead of this laggy thing
            if(_delta > 200) {       // To make sure it doesn't freeze
                _isRunning = false;
                break;
            }
            
            _handleKeyStates();
            _onLoop();
            
            _updates++;
            _delta--;
        }
        
        _onRender();
        
        _frames++;
        
        if(SDL_GetTicks() - _timer > 1000) {
            _timer += 1000;
            _title.str("");
            _title /* << _intro << " | " */ << _updates << " ups, " << _frames << " fps";
            //instance.window.setTitle(_title.str());
            if(instance.player)
                instance.player->say(_title.str(), "TESTFONT", ChatBubbleType::SAY);
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
        NFile::log(LogType::ERROR, "Window.onInit failed: ", SDL_GetError());
        return -1;
    }
    instance.camera.onInit(&instance.window);
    
    NFile::loadMap("resources/map/testMap1.map", &instance);
    
    CEntity* tempEntity = new CEntity(Box{300, 0, 80, 140}, "playerPink");          // Temp player
    tempEntity->addComponent(new EMovable(tempEntity));
    tempEntity->addComponent(new ELiving(tempEntity));
    tempEntity->getComponent<EMovable>()->jumpPower = 15.0;
    tempEntity->spriteStateTypes[SpriteStateTypes::ASCENDING] = "playerPinkRunning";
    tempEntity->spriteStateTypes[SpriteStateTypes::DESCENDING] = "playerPinkRunning";
    tempEntity->spriteFollowsCollisionBox = false;
    instance.player = tempEntity;
    instance.entityManager.addEntity(instance.player);
    
    auto tempEntity2 = new CEntity(Box{450, 0, 80, 140}, "playerPink");                   // Temp enemy
    tempEntity2->addComponent(new EMovable(tempEntity2));
    tempEntity2->addComponent(new ENpc(tempEntity2));
    tempEntity2->addComponent(new ELiving(tempEntity2));
    tempEntity2->getComponent<EMovable>()->setMovementState(MovementState::SNEAKING_MOVEMENT);
    tempEntity2->getComponent<ENpc>()->setTarget(instance.player);
    tempEntity2->spriteStateTypes[SpriteStateTypes::ASCENDING] = "playerPinkRunning";
    tempEntity2->spriteStateTypes[SpriteStateTypes::DESCENDING] = "playerPinkRunning";
    tempEntity2->spriteFollowsCollisionBox = false;
    instance.entityManager.addEntity(tempEntity2);
    
    tempEntity = new CEntity(Box{0, 465, 5000, 30}, SDL_Color{255, 0, 0, 0});       // Temp platforms
    tempEntity->addCollisionLayer(-129);
    tempEntity->addProperty(EntityProperty::STATIC);
    instance.entityManager.addEntity(tempEntity);
    
    tempEntity = new CEntity(Box{0, -20, 30, 500}, SDL_Color{255, 0, 0, 0});
    tempEntity->addCollisionLayer(15);
    tempEntity->addProperty(EntityProperty::STATIC);
    instance.entityManager.addEntity(tempEntity);
    
    instance.camera.setTarget(instance.player);
    
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

void CGame::_handleKeyStates() {
    
    if(!isFocused)
        return;
    
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    // Movement
    
    if(!instance.player)
        return;
    
    EMovable* movable = instance.player->getComponent<EMovable>();
    if(movable) {
        if(keystate[SDL_SCANCODE_D]) {
            movable->goRight();
        }
        if(keystate[SDL_SCANCODE_A]) {
            movable->goLeft();
        }
        
        if(keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_SPACE]) {
            movable->goUp();
        }
        
        if(keystate[SDL_SCANCODE_S]) {
            movable->goDown();
        }
    }
    
    // Other
    
    if(NMouse::leftMouseButtonPressed()) { // damage particle
        int mousePosX = instance.player->body.getX() + instance.player->body.getW() / 2 - NMouse::relativeMouseX(&instance.camera);
        int mousePosY = instance.player->body.getY() - 100 - NMouse::relativeMouseY(&instance.camera);
        float angle = atan2(mousePosY, mousePosX);
        
        const int velocityX = -(cos(angle) * 100);
        const int velocityY = -(sin(angle) * 100);
        
//        int middleX = instance.player->body.getX() + instance.player->body.getW() / 2;
//        int middleY = instance.player->body.getY() + instance.player->body.getH() / 2;
//        
//        int spawnX = middleX + cos(angle) * -instance.player->body.getH() / 2;
//        int spawnY = middleY + sin(angle) * -instance.player->body.getH() / 2;
        
        //instance.entityManager.addParticleEmitter(sf::IntRect{instance.player->body.getX(), instance.player->body.getY() - 100, 10, 10}, SDL_Color{ (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), 0}, ParticleTypes::UTILITY_PARTICLE, 1, 1, 1, 10, ParticleVelocity{(float)velocityX, (float)velocityY});
        
        CEntity* tempParticle = new CEntity(Box{instance.player->body.getX() + instance.player->body.getW() / 2, instance.player->body.getY() - 100, 4, 4}, SDL_Color{ (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0});
        tempParticle->addComponent(new EUtilityParticle(tempParticle, instance.player, BasicUtilities::DAMAGE, 10));
        tempParticle->body.velX = velocityX;
        tempParticle->body.velY = velocityY;
        instance.entityManager.addParticle(tempParticle);
        
        instance.camera.addCameraShake(1.1);
    }
    
    if(NMouse::rightMouseButtonPressed()) {   // heal particle
        int mousePosX = instance.player->body.getX() + instance.player->body.getW() / 2 - NMouse::relativeMouseX(&instance.camera);
        int mousePosY = instance.player->body.getY() - 100 - NMouse::relativeMouseY(&instance.camera);
        float angle = atan2(mousePosY, mousePosX);
        
        const float velocityX = -(cos(angle) * 100);
        const float velocityY = -(sin(angle) * 100);
        
        //instance.entityManager.addParticleEmitter(sf::IntRect{instance.player->body.getX(), instance.player->body.getY() - 100, 10, 10}, SDL_Color{ (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), 0}, ParticleTypes::UTILITY_PARTICLE, 1, 1, 1, 10, ParticleVelocity{(float)velocityX, (float)velocityY});
        
        CEntity* tempParticle = new CEntity(Box{instance.player->body.getX() + instance.player->body.getW() / 2, instance.player->body.getY() - 100, 4, 4}, SDL_Color{ (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0});
        tempParticle->addComponent(new EUtilityParticle(tempParticle, instance.player, BasicUtilities::HEAL, 10));
        tempParticle->body.velX = velocityX;
        tempParticle->body.velY = velocityY;
        instance.entityManager.addParticle(tempParticle);
        
        instance.camera.setTarget(tempParticle);
    }
}

void CGame::_onEvent() {
    
    //if(event->key.repeat != 0) return;
    
    auto event = &_event;
    
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
            
//        case sf::Event::Resized:
//            //instance.window.updateView(event->size.width, event->size.height);
//            //instance.window.setSize(event->size.width, event->size.height);
//            break;
            
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym) {
                    
                case keyMap::EXIT:
                    _isRunning = false;
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
                    instance.player->body._rect.x = 300;
                    instance.player->body._rect.y = 1000000;
                    break;
                    
                case SDLK_7:
                    instance.camera.addCameraShake(100.0);
                    break;
                    
                case keyMap::SNEAK:
                    if(instance.player) {
                        EMovable* movable = instance.player->getComponent<EMovable>();
                        if(movable)
                            movable->setMovementState(MovementState::SNEAKING_MOVEMENT);
                    }
                    break;
                    
                case SDLK_LCTRL:
                    if(instance.player) {
                        EMovable* movable = instance.player->getComponent<EMovable>();
                        if(movable)
                            movable->setMovementState(MovementState::RUNNING_MOVEMENT);
                    }
                    break;
                    
                case keyMap::BLOCK:
                {
                    CEntity* temp = instance.entityManager.addEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 40, 40}, SDL_Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                }
                    break;
                    
                case keyMap::RESET:
                {
                    auto tempNpc = new CEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 60, 164}, "player");
                    tempNpc->addComponent(new ENpc(tempNpc));
                    tempNpc->addComponent(new EMovable(tempNpc));
                    tempNpc->addComponent(new ENpc(tempNpc));
                    tempNpc->addComponent(new ELiving(tempNpc));
                    tempNpc->getComponent<EMovable>()->setMovementState(MovementState::SNEAKING_MOVEMENT);
                    tempNpc->getComponent<ENpc>()->setTarget(instance.player);
                    tempNpc->spriteStateTypes[SpriteStateTypes::ASCENDING] = "enemyJumping";
                    tempNpc->spriteFollowsCollisionBox = false;
                    instance.entityManager.addEntity(tempNpc);
                }
                    break;
                    
                case SDLK_h:
                {
                    auto tempNpc = new CEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 32 * 4, 32 * 4}, "yrl");
                    tempNpc->addComponent(new ENpc(tempNpc));
                    tempNpc->addComponent(new ELiving(tempNpc));
                    tempNpc->getComponent<ENpc>()->setTarget(instance.player);
                    instance.entityManager.addEntity(tempNpc);
                }
                    break;
                    
                case keyMap::TOGGLE_NOCLIP:
                    if(instance.player) {
                        EMovable* movable = instance.player->getComponent<EMovable>();
                        if(movable)
                            movable->toggleNoclip();
                    }
                    break;
                case keyMap::LOAD_ASSETS:
                    NFile::loadMap("resources/map/testMap1.map", &instance);
                    instance.camera.onInit(&instance.window);
                    break;
                case keyMap::TOGGLE_HIDDEN:
                    if(instance.player)
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
                        return -1;
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
                    if(instance.player)
                        instance.player->say(text, "TESTFONT", ChatBubbleType::SAY);
                    
                    text = "";
                    for(int i = 0; i < 50; i++) {
                        text += alphanum[rand() % (sizeof(alphanum) - 1)];
                    }
                    //i1nstance.entityManager.getEntity("n:bush")->say(text, "TESTFONT", ChatBubbleType::YELL);
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
                    if(instance.player) {
                        EMovable* movable = instance.player->getComponent<EMovable>();
                        if(movable)
                            movable->setMovementState(MovementState::WALKING_MOVEMENT);
                    }
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
//                auto tempTarget = instance.entityManager.getEntityAtCoordinate(NMouse::relativeMouseX(instance.window.getWindow(), &instance.camera), NMouse::relativeMouseY(instance.window.getWindow(), &instance.camera));
//                if(tempTarget != nullptr && instance.seeker != nullptr) {
//                    instance.seeker->setTarget(tempTarget);
//                    instance.seeker->say("Target confirmed: " + instance.entityManager.getNameOfEntity(tempTarget), "TESTFONT", ChatBubbleType::SAY);
//                }
            }
            break;
    }
    
}

void CGame::_onLoop() {
    instance.entityManager.onLoop(&instance);
    instance.camera.onLoop();
}

void CGame::_onRender() {
    
    SDL_SetRenderDrawColor(instance.window.getRenderer(), 245, 245, 245, 255);
    SDL_RenderClear(instance.window.getRenderer());
    
    instance.entityManager.onRender(&instance.window, &instance.camera);
    
    SDL_RenderPresent(instance.window.getRenderer());
}

int CGame::_onCleanup() {
    instance.entityManager.onCleanup();
    CAssetManager::onCleanup();
    instance.window.onCleanup();
    
    return 0;
}