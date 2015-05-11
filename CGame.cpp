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
#include <SFML/Graphics.hpp>
#include "CText.h"
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#include "ResourcePath.hpp"
#endif
#include <SFML/Network.hpp>

CGame::CGame() :
_intro("Physics"),
//WIDTH(640), HEIGHT(480), BPP(32), camera(WIDTH, HEIGHT),
_lastTime(_clock.getElapsedTime().asMilliseconds()), _timer(_clock.getElapsedTime().asMilliseconds()),
_ns(1000.0f / (float)GAMEINTERVAL), _delta(0), _frames(0), _updates(0) {
}

CGame::~CGame() {
}

int CGame::onExecute() {
    
    std::cout << "Initializing game...\n";
    
    switch(_onInit()){
        case -1:
            std::cout << "Initializing failed!\n";
            _running = false;
        case 0:
            std::cout << "Initializing succesful!\n";
    }
    
    std::cout << "Starting game...\n";
    
    while(instance.window.getWindow()->isOpen()) {
        sf::Event event;
        while(instance.window.getWindow()->pollEvent(event)){
            _onEvent(&event);
        }
        
        //std::cout << "CameraX: " << camera.offsetX() << ", CameraY: " << camera.offsetY() << " \n";
        
        float now = _clock.getElapsedTime().asMilliseconds();
        _delta += (now - _lastTime) / _ns;
        _lastTime = now;
        
        while(_delta >= 1) {
            //std::cout << (int)floor(delta) << " | ";
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
        
        if(_clock.getElapsedTime().asMilliseconds() - _timer > 1000) {
            _timer += 1000;
            _title.str("");
            _title << _intro << " | " << _updates << " ups, " << _frames << " fps";
            instance.window.setTitle(_title.str());
            _updates = 0;
            _frames = 0;
        }
        
        //SDL_Delay(7);
        
    }
    
    std::cout << "Ending game...\n";
    
    return _onCleanup();
    
}

int CGame::_onInit() {
    
    _initRelativePaths();
    
    srand((sf::Uint16)time(nullptr));
    
    if(instance.window.onInit(_intro, SCREEN_WIDTH, SCREEN_HEIGHT))
        return -1;
    instance.camera.onInit(&instance.window);
    
    instance.window.getRenderTexture()->create(SCREEN_WIDTH, SCREEN_HEIGHT);                // Draw unto a texture for applying shaders later
    instance.window.getSprite()->setTexture(instance.window.getRenderTexture()->getTexture());
    
    _initAssets();
    
    instance.player = new CPlayer(sf::IntRect{30, 30, 60, 164}, "player");
    instance.entityManager.addEntity(instance.player, "m:player");                                                // Layer system: z -> a. visible to nonvisible
    instance.camera.setTarget(instance.player);
    
    instance.entityManager.addEntity(sf::IntRect{0 - 30 / 2, 480 - 30 / 2, 5000, 30}, sf::Color{255, 0, 0, 0});
    instance.entityManager.addEntity(sf::IntRect{0 - 30 / 2, 480 - 500, 30, 500}, sf::Color{255, 0, 0, 0});
    instance.entityManager.addEntity(sf::IntRect{276, 229, 23 * 4, 59 * 4}, "tree", "l:tree");
    instance.entityManager.getEntity("l:tree")->removeProperty(EntityProperty::COLLIDABLE);
    instance.entityManager.getEntity("l:tree")->addProperty(EntityProperty::STATIC);
    instance.entityManager.getEntity("l:tree")->setShaderKey("");
    instance.entityManager.addEntity(sf::IntRect{200, 357, 60 * 2, 54 * 2}, "bush", "n:bush");
    instance.entityManager.getEntity("n:bush")->removeProperty(EntityProperty::COLLIDABLE);
    instance.entityManager.getEntity("n:bush")->addProperty(EntityProperty::STATIC);
    instance.entityManager.getEntity("n:bush")->addProperty(EntityProperty::STATIC);
    instance.entityManager.getEntity("n:bush")->setShaderKey("");

    return 0;
}

void CGame::_initAssets() {
    CAssetManager::addSpriteSheet("MAIN", "resources/gfx.png");
    CAssetManager::addSpriteSheet("MAIN2", "resources/gfx2.png");
    CAssetManager::addSprite("player", "MAIN2", sf::IntRect{144,396,60,164});
    CAssetManager::addSprite("bush", "MAIN", sf::IntRect{160, 91, 30, 28});
    CAssetManager::addSprite("tree", "MAIN", sf::IntRect{7,64,23,59});
    CAssetManager::addSpriteSheet("BG", "resources/bg.png");
    CAssetManager::addSprite("background", "BG", sf::IntRect{0,0,128,64});
    CAssetManager::addFont("TESTFONT", "resources/font.ttf");
    CAssetManager::addShader("SHADER1", "resources/light.frag", sf::Shader::Type::Fragment);
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
        std::cout << "Current Path: " << path << std::endl;
    #endif
    // ----------------------------------------------------------------------------
}

void CGame::_handleKeyStates() {
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::RIGHT)) {
        instance.player->goRight();
    }
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::LEFT)) {
        instance.player->goLeft();
    }
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::UP)) {
        instance.player->goUp();
    }
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::DOWN)) {
        instance.player->goDown();
    }
}

void CGame::_onEvent(sf::Event* event) {
    
    
    
    //if(event->key.repeat != 0) return;
    
    switch(event->type) {
        case sf::Event::Closed:
            instance.window.getWindow()->close();
            break;
            
        case sf::Event::Resized:
            instance.window.updateView(event->size.width, event->size.height);
            break;
            
        case sf::Event::KeyPressed:
            switch(event->key.code) {
                    
                case keyMap::EXIT:
                    instance.window.getWindow()->close();
                    break;
                    
                case keyMap::SNEAK:
                    instance.player->isSneaking = true;
                    break;
                    
                case keyMap::BLOCK:
                {
                    CEntity* temp = instance.entityManager.addEntity(sf::IntRect{NMouse::relativeMouseX(instance.window.getWindow(), &instance.camera) - 30 / 2, NMouse::relativeMouseY(instance.window.getWindow(), &instance.camera) - 30 / 2, 40, 40}, sf::Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                }
                    break;
                    
                case keyMap::PARTICLEEM:
                    instance.entityManager.addParticleEmitter(sf::IntRect{NMouse::relativeMouseX(instance.window.getWindow(), &instance.camera) - 4 / 2, NMouse::relativeMouseY(instance.window.getWindow(), &instance.camera) - 4 / 2, 10, 10}, sf::Color{ (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), 0}, 20, 2, 4, 6, 0.3);
                    break;
                    
                case keyMap::RESET:
                    *instance.player = CPlayer(sf::IntRect{30, 30, 30, 30}, sf::Color{255, 255, 0, 255});
                    break;
                    
                case keyMap::TOGGLE_NOCLIP:
                    instance.player->toggleProperty(EntityProperty::COLLIDABLE);
                    instance.player->toggleProperty(EntityProperty::FLYING);
                    break;
                case keyMap::LOAD_ASSETS:
                    _initAssets();
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
                    //player->toggleProperty(EntityProperty::STATIC);
                    instance.window.newWindow(_intro, 600, 400);
                    instance.camera.onInit(&instance.window);
                    CAssetManager::onCleanup();
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
                    instance.player->say(text, "TESTFONT", &instance.entityManager, ChatBubbleType::SAY);
                    
                    text = "";
                    for(int i = 0; i < 50; i++) {
                        text += alphanum[rand() % (sizeof(alphanum) - 1)];
                    }
                    instance.entityManager.getEntity("n:bush")->say(text, "TESTFONT", &instance.entityManager, ChatBubbleType::YELL);
                }
                    break;
                    
                case keyMap::TARGET_PLAYER:
                    instance.camera.setTarget(instance.player);
                    break;
                case keyMap::TARGET_BLOCK:
                    instance.camera.setTarget(instance.entityManager.getEntity("n:bush"));
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
        
        case sf::Event::KeyReleased:
            switch(event->key.code) {
                case keyMap::SNEAK:
                    instance.player->isSneaking = false;
                    break;
                    
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
}

void CGame::_onLoop() {
    
    // Send
    
    sf::UdpSocket socket;
    if(socket.bind(1337) != sf::Socket::Done) { /* error */ }
    sf::IpAddress adress = sf::IpAddress::LocalHost;
    unsigned short port = 1234;
    std::string thing = std::to_string(instance.entityManager.getEntity("m:player")->body.getX());
    char data[thing.size()];
    std::strcpy(data, thing.c_str());
    if(socket.send(data, thing.size(), adress, port) != sf::Socket::Done) { /* error */ }
    
    
    
    instance.entityManager.onLoop();
    instance.camera.onLoop();
}

//void CGame::_onRender() {
//    
//    sf::View view1(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
//    view1.setViewport(sf::FloatRect(0, 0, 1, 1));
//    sf::View view2(sf::FloatRect(0.5f, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
//    view2.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
//    view2.zoom(2.0f);
//    
//    instance.window.getRenderTexture()->clear();
//    
//    instance.window.getWindow()->setView(view1);
//    instance.window.getWindow()->clear();
//    NSurface::renderRect(sf::IntRect{0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, instance.window.getRenderTexture(), 255, 255, 255);
//    instance.entityManager.onRender(instance.window.getRenderTexture(), &instance.camera);
//    
////    instance.window.getWindow()->setView(view2);
////    instance.entityManager.onRender(instance.window.getRenderTexture(), &instance.camera);
//    
//    instance.window.getRenderTexture()->display();
//    instance.window.getWindow()->display();
//    
//}

void CGame::_onRender() {
    
    instance.window.getWindow()->clear();
    instance.window.getRenderTexture()->clear();
    
    NSurface::renderRect(sf::IntRect{0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, *instance.window.getRenderTexture(), 255, 255, 255);
    instance.entityManager.onRender(&instance.window, &instance.camera);
    
    instance.window.getRenderTexture()->display();
    instance.window.getWindow()->draw(*instance.window.getSprite());
    instance.window.getWindow()->display();
    
}

int CGame::_onCleanup() {
    instance.entityManager.onCleanup();
    CAssetManager::onCleanup();
    instance.window.onCleanup();
    
    return 0;
}