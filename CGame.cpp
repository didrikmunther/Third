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
    
    while(_window.getWindow()->isOpen()) {
        sf::Event event;
        while(_window.getWindow()->pollEvent(event)){
            _onEvent(&event);
        }
        
        //std::cout << "CameraX: " << camera.offsetX() << ", CameraY: " << camera.offsetY() << " \n";
        
        float now = _clock.getElapsedTime().asMilliseconds();
        _delta += (now - _lastTime) / _ns;
        _lastTime = now;
        
        while(_delta >= 1) {
            //std::cout << (int)floor(delta) << " | ";
            if(_delta > 20) {       // To make sure it doesn't freeze
                _entityManager.particleCleanup();
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
            _window.setTitle(_title.str());
            _updates = 0;
            _frames = 0;
        }
        
        //SDL_Delay(7);
        
    }
    
    std::cout << "Ending game...\n";
    
    return _onCleanup();
    
}

int CGame::_onInit() {
    
    srand((sf::Uint16)time(nullptr));
    
    if(_window.onInit(_intro, SCREEN_WIDTH, SCREEN_HEIGHT))
        return -1;
    _camera.onInit(&_window);
    
    _assetManager.addSpriteSheet("MAIN", "resources/gfx.png");
    _assetManager.addSpriteSheet("MAIN2", "resources/gfx2.png");
    _assetManager.addSprite("player", "MAIN2", sf::IntRect{144,396,60,164});
    _assetManager.addSprite("bush", "MAIN", sf::IntRect{160, 91, 30, 28});
    _assetManager.addSprite("tree", "MAIN", sf::IntRect{7,64,23,59});
    _assetManager.addSpriteSheet("BG", "resources/bg.png");
    _assetManager.addSprite("background", "BG", sf::IntRect{0,0,128,64});
    _assetManager.addFont("TESTFONT", "resources/font.ttf");
    
    _player = new CPlayer(sf::IntRect{30, 30, 60, 164}, "player", &_assetManager);
    _entityManager.addEntity(_player, "m:player");                                                // Layer system: z -> a. visible to nonvisible
    _camera.setTarget(_player);
    
    _entityManager.addEntity(sf::IntRect{0 - 30 / 2, 480 - 30 / 2, 5000, 30}, sf::Color{255, 0, 0, 0});
    _entityManager.addEntity(sf::IntRect{0 - 30 / 2, 480 - 500, 30, 500}, sf::Color{255, 0, 0, 0});
    auto tree = _entityManager.addEntity(sf::IntRect{276, 229, 23 * 4, 59 * 4}, "tree", &_assetManager, "l:tree");
    tree->removeProperty(EntityProperty::COLLIDABLE);
    tree->addProperty(EntityProperty::STATIC);
    _block = _entityManager.addEntity(sf::IntRect{200, 357, 60 * 2, 54 * 2}, "bush", &_assetManager, "n:bush");
    _block->removeProperty(EntityProperty::COLLIDABLE);
    _block->addProperty(EntityProperty::STATIC);

    return 0;
}

void CGame::_handleKeyStates() {
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::RIGHT)) {
        _player->goRight();
    }
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::LEFT)) {
        _player->goLeft();
    }
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::UP)) {
        _player->goUp();
    }
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::DOWN)) {
        _player->goDown();
    }
}

void CGame::_onEvent(sf::Event* event) {
    
    
    
    //if(event->key.repeat != 0) return;
    
    switch(event->type) {
        case sf::Event::Closed:
            _window.getWindow()->close();
            break;
            
        case sf::Event::KeyPressed:
            switch(event->key.code) {
                    
                case keyMap::EXIT:
                    _window.getWindow()->close();
                    break;
                    
                case keyMap::SNEAK:
                    _player->isSneaking = true;
                    break;
                    
                case keyMap::BLOCK:
                {
                    CEntity* temp = _entityManager.addEntity(sf::IntRect{NMouse::relativeMouseX(_window.getWindow(), &_camera) - 30 / 2, NMouse::relativeMouseY(_window.getWindow(), &_camera) - 30 / 2, 40, 40}, sf::Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                }
                    break;
                    
                case keyMap::PARTICLEEM:
                    _entityManager.addParticleEmitter(sf::IntRect{NMouse::relativeMouseX(_window.getWindow(), &_camera) - 4 / 2, NMouse::relativeMouseY(_window.getWindow(), &_camera) - 4 / 2, 10, 10}, sf::Color{ (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), 0}, 20, 2, 4, 6, 0.3);
                    break;
                    
                case keyMap::RESET:
                    *_player = CPlayer(sf::IntRect{30, 30, 30, 30}, sf::Color{255, 255, 0, 255});
                    break;
                    
                case keyMap::TOGGLE_NOCLIP:
                    _player->toggleProperty(EntityProperty::COLLIDABLE);
                    _player->toggleProperty(EntityProperty::FLYING);
                    break;
                case keyMap::LOAD_ASSETS:
                    _assetManager.addSpriteSheet("MAIN", "resources/gfx.png");
                    _assetManager.addSpriteSheet("MAIN2", "resources/gfx2.png");
                    _assetManager.addSprite("player", "MAIN2", sf::IntRect{144,396,60,164});
                    _assetManager.addSprite("bush", "MAIN", sf::IntRect{160, 91, 30, 28});
                    _assetManager.addSprite("tree", "MAIN", sf::IntRect{7,64,23,59});
                    _assetManager.addSpriteSheet("BG", "resources/bg.png");
                    _assetManager.addSprite("background", "BG", sf::IntRect{0,0,128,64});
                    _assetManager.addFont("TESTFONT", "resources/font.ttf");
                    break;
                case keyMap::TOGGLE_HIDDEN:
                    _player->toggleProperty(EntityProperty::HIDDEN);
                    break;
                case keyMap::TOGGLE_COLLISION_BOUNDS:
                    //player->toggleProperty(EntityProperty::FLYING);
                    _entityManager.toggleRenderFlag(RenderFlags::COLLISION_BORDERS);
                    break;
                case keyMap::NEW_WINDOW:
                {
                    //player->toggleProperty(EntityProperty::STATIC);
                    _window.newWindow(_intro, 600, 400);
                    _camera.onInit(&_window);
                    _assetManager.onCleanup();
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
                    _player->say(text, "TESTFONT", &_assetManager, &_entityManager, ChatBubbleType::SAY);
                    
                    text = "";
                    for(int i = 0; i < 50; i++) {
                        text += alphanum[rand() % (sizeof(alphanum) - 1)];
                    }
                    _block->say(text, "TESTFONT", &_assetManager, &_entityManager, ChatBubbleType::YELL);
                }
                    break;
                    
                case keyMap::TARGET_PLAYER:
                    _camera.setTarget(_player);
                    break;
                case keyMap::TARGET_BLOCK:
                    _camera.setTarget(_block);
                    break;
                    
                case keyMap::CHANGE_CAMERA_SWAY_UP:
                        _camera.cameraSway += 10;
                    break;
                case keyMap::CHANGE_CAMERA_SWAY_DOWN:
                    if(_camera.cameraSway <= 10)
                        _camera.cameraSway = 1;
                    else
                        _camera.cameraSway -= 10;
                    break;
                    
                default:
                    break;
                    
            }
            break;
        
        case sf::Event::KeyReleased:
            switch(event->key.code) {
                case keyMap::SNEAK:
                    _player->isSneaking = false;
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
    _entityManager.onLoop();
    _camera.onLoop();
}

void CGame::_onRender() {
    
    sf::View view1(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    view1.setViewport(sf::FloatRect(0, 0, 1, 1));
    sf::View view2(sf::FloatRect(0.5f, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    view2.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
    view2.zoom(2.0f);
    
    _window.getWindow()->setView(view1);
    _window.getWindow()->clear();
    NSurface::renderRect(sf::IntRect{0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, _window.getWindow(), 255, 255, 255);
    _entityManager.onRender(_window.getWindow(), &_camera);
    
    _window.getWindow()->setView(view2);
    _entityManager.onRender(_window.getWindow(), &_camera);
    
    _window.getWindow()->display();
    
}

int CGame::_onCleanup() {
    _entityManager.onCleanup();
    _assetManager.onCleanup();
    _window.onCleanup();
    
    return 0;
}