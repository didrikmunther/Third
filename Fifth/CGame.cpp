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
#include "CEnemy.h"
#include "CUtilityParticle.h"
#include "CSpriteContainer.h"
#include <fstream>

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
        }
        
        _onRender();
        
        _frames++;
        
        if(SDL_GetTicks() - _timer > 1000) {
            _timer += 1000;
            _title.str("");
            _title /* << _intro << " | " */ << _updates << " ups, " << _frames << " fps";
            //instance.window.setTitle(_title.str());
            instance.entityManager.getEntity("n:bush")->say(_title.str(), "TESTFONT", ChatBubbleType::SAY);
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
    
    /*
     Entity = 0,
     Particle,   // 1
     Movable,    // 2
     UtilityParticle, // 3
     Living,     // 4
     Npc,        // 5
     Player,     // 6
     Enemy       // 7
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
    
    if(keystate[SDL_SCANCODE_D]) {
        instance.player->goRight();
    }
    if(keystate[SDL_SCANCODE_A]) {
        instance.player->goLeft();
    }
    
    if(keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_SPACE]) {
        instance.player->goUp();
    }
    
    if(keystate[SDL_SCANCODE_S]) {
        instance.player->goDown();
    }
    
    // Other
    
    if(NMouse::leftMouseButtonPressed()) { // damage particle
        int mousePosX = instance.player->body.getX() - NMouse::relativeMouseX(&instance.camera);
        int mousePosY = instance.player->body.getY() - 100 - NMouse::relativeMouseY(&instance.camera);
        float angle = atan2(mousePosY, mousePosX);
        
        const int velocityX = -(cos(angle) * 100);
        const int velocityY = -(sin(angle) * 100);
        
        //instance.entityManager.addParticleEmitter(sf::IntRect{instance.player->body.getX(), instance.player->body.getY() - 100, 10, 10}, SDL_Color{ (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), 0}, ParticleTypes::UTILITY_PARTICLE, 1, 1, 1, 10, ParticleVelocity{(float)velocityX, (float)velocityY});
        
        CUtilityParticle* tempParticle = new CUtilityParticle(Box{instance.player->body.getX(), instance.player->body.getY() - 100, 4, 4}, SDL_Color{ (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0}, instance.player, BasicUtilities::DAMAGE, 10);
        tempParticle->body.velX = velocityX;
        tempParticle->body.velY = velocityY;
        instance.entityManager.addParticle(tempParticle);
    }
    
    if(NMouse::rightMouseButtonPressed()) {   // heal particle
        int mousePosX = instance.player->body.getX() - NMouse::relativeMouseX(&instance.camera);
        int mousePosY = instance.player->body.getY() - 100 - NMouse::relativeMouseY(&instance.camera);
        float angle = atan2(mousePosY, mousePosX);
        
        const float velocityX = -(cos(angle) * 100);
        const float velocityY = -(sin(angle) * 100);
        
        //instance.entityManager.addParticleEmitter(sf::IntRect{instance.player->body.getX(), instance.player->body.getY() - 100, 10, 10}, SDL_Color{ (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), 0}, ParticleTypes::UTILITY_PARTICLE, 1, 1, 1, 10, ParticleVelocity{(float)velocityX, (float)velocityY});
        
        CUtilityParticle* tempParticle = new CUtilityParticle(Box{instance.player->body.getX(), instance.player->body.getY() - 100, 20, 20}, SDL_Color{ (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0}, instance.player, BasicUtilities::HEAL, 10);
        tempParticle->body.velX = velocityX;
        tempParticle->body.velY = velocityY;
        instance.entityManager.addParticle(tempParticle);
    }
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
            
//        case sf::Event::Resized:
//            //instance.window.updateView(event->size.width, event->size.height);
//            //instance.window.setSize(event->size.width, event->size.height);
//            break;
            
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym) {
                    
                case keyMap::EXIT:
                    _isRunning = false;
                    break;
                    
                case SDLK_8:
                {
                    float angle = 0;
                    int particles = 90;
                    int velocity = 50;
                    
                    for(int i = 0; i < particles; i++) {
                        angle += 0.0174532925f * (360.0f / particles);
                        
                        const float velocityX = -(cos(angle) * velocity);
                        const float velocityY = -(sin(angle) * velocity);
                        
                        CUtilityParticle* tempParticle = new CUtilityParticle(Box{instance.player->body.getX() + instance.player->body.getW() / 2, instance.player->body.getY() + instance.player->body.getH() / 2, 10, 10}, SDL_Color{ (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0}, instance.player, BasicUtilities::DAMAGE, 10);
                        tempParticle->body.velX = velocityX;
                        tempParticle->body.velY = velocityY;
                        instance.entityManager.addParticle(tempParticle);
                    }
                }
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
                    
                case keyMap::SNEAK:
                    instance.player->setMovementState(MovementState::SNEAKING_MOVEMENT);
                    break;
                    
                case SDLK_LCTRL:
                    instance.player->setMovementState(MovementState::RUNNING_MOVEMENT);
                    break;
                    
                case keyMap::BLOCK:
                {
                    CEntity* temp = instance.entityManager.addEntity(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 40, 40}, SDL_Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                }
                    break;
                    
                case keyMap::RESET:
                {
                    auto tempNpc = new CEnemy(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 60, 164}, "player");
                    tempNpc->setTarget(instance.player);
                    tempNpc->spriteStateTypes[SpriteStateTypes::ASCENDING] = "enemyJumping";
                    tempNpc->spriteFollowsCollisionBox = false;
                    instance.entityManager.addEntity(tempNpc);
                }
                    break;
                    
                case SDLK_h:
                {
                    auto tempNpc = new CEnemy(Box{NMouse::relativeMouseX(&instance.camera), NMouse::relativeMouseY(&instance.camera), 32 * 4, 32 * 4}, "yrl");
                    tempNpc->setTarget(instance.player);
                    instance.entityManager.addEntity(tempNpc);
                }
                    break;
                    
                case keyMap::TOGGLE_NOCLIP:
                    instance.player->toggleNoclip();
                    break;
                case keyMap::LOAD_ASSETS:
                    NFile::loadMap("resources/map/testMap1.map", &instance);
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
                    NFile::loadMap("resources/map/testMap1.map", &instance);
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
                    instance.player->setMovementState(MovementState::WALKING_MOVEMENT);
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
    instance.entityManager.onLoop();
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
    
    return 0;
}