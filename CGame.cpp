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
intro("Physics"),
//WIDTH(640), HEIGHT(480), BPP(32), camera(WIDTH, HEIGHT),
lastTime(clock.getElapsedTime().asMilliseconds()), timer(clock.getElapsedTime().asMilliseconds()),
ns(1000.0f / (float)GAMEINTERVAL), delta(0), frames(0), updates(0) {
}

CGame::~CGame() {
}

int CGame::onExecute() {
    
    std::cout << "Initializing game...\n";
    
    switch(onInit()){
        case -1:
            std::cout << "Initializing failed!\n";
            running = false;
        case 0:
            std::cout << "Initializing succesful!\n";
    }
    
    std::cout << "Starting game...\n";
    
    while(window.getWindow()->isOpen()) {
        sf::Event event;
        while(window.getWindow()->pollEvent(event)){
            onEvent(&event);
        }
        
        //std::cout << "CameraX: " << camera.offsetX() << ", CameraY: " << camera.offsetY() << " \n";
        
        float now = clock.getElapsedTime().asMilliseconds();
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
        
        if(clock.getElapsedTime().asMilliseconds() - timer > 1000) {
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
    
    srand((sf::Uint16)time(nullptr));
    
    if(window.onInit(intro, SCREEN_WIDTH, SCREEN_HEIGHT))
        return -1;
    camera.onInit(&window);
    
    assetManager.addSpriteSheet("MAIN", "resources/gfx.png");
    assetManager.addSpriteSheet("MAIN2", "resources/gfx2.png");
    assetManager.addSprite("player", "MAIN2", sf::IntRect{144,396,60,164});
    assetManager.addSprite("bush", "MAIN", sf::IntRect{160, 91, 30, 28});
    assetManager.addSprite("tree", "MAIN", sf::IntRect{7,64,23,59});
    assetManager.addSpriteSheet("BG", "resources/bg.png");
    assetManager.addSprite("background", "BG", sf::IntRect{0,0,128,64});
    assetManager.addFont("TESTFONT", "resources/font.ttf");
    
    player = new CPlayer(sf::IntRect{30, 30, 60, 164}, "player", &assetManager);
    entityManager.addEntity(player, "m:player");                                                // Layer system: z -> a. visible to nonvisible
    camera.setTarget(player);
    
    entityManager.addEntity(sf::IntRect{0 - 30 / 2, 480 - 30 / 2, 5000, 30}, sf::Color{255, 0, 0, 0});
    entityManager.addEntity(sf::IntRect{0 - 30 / 2, 480 - 500, 30, 500}, sf::Color{255, 0, 0, 0});
    auto tree = entityManager.addEntity(sf::IntRect{276, 229, 23 * 4, 59 * 4}, "tree", &assetManager, "l:tree");
    tree->removeProperty(EntityProperty::COLLIDABLE);
    tree->addProperty(EntityProperty::STATIC);
    block = entityManager.addEntity(sf::IntRect{200, 357, 60 * 2, 54 * 2}, "bush", &assetManager, "n:bush");
    block->removeProperty(EntityProperty::COLLIDABLE);
    block->addProperty(EntityProperty::STATIC);

    return 0;
}

void CGame::handleKeyStates() {
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::RIGHT)) {
        player->goRight();
    }
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::LEFT)) {
        player->goLeft();
    }
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::UP)) {
        player->goUp();
    }
    
    if(sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keyMap::DOWN)) {
        player->goDown();
    }
}

void CGame::onEvent(sf::Event* event) {
    
    
    
    //if(event->key.repeat != 0) return;
    
    switch(event->type) {
        case sf::Event::Closed:
            window.getWindow()->close();
            break;
            
        case sf::Event::KeyPressed:
            switch(event->key.code) {
                    
                case keyMap::EXIT:
                    window.getWindow()->close();
                    break;
                    
                case keyMap::SNEAK:
                    player->isSneaking = true;
                    break;
                    
                case keyMap::BLOCK:
                {
                    CEntity* temp = entityManager.addEntity(sf::IntRect{NMouse::relativeMouseX(&camera) - 30 / 2, NMouse::relativeMouseY(&camera) - 30 / 2, 40, 40}, sf::Color{0, 0, 255, 0});
                    temp->addProperty(EntityProperty::STATIC);
                }
                    break;
                    
                case keyMap::PARTICLEEM:
                    entityManager.addParticleEmitter(sf::IntRect{NMouse::relativeMouseX(&camera) - 4 / 2, NMouse::relativeMouseY(&camera) - 4 / 2, 10, 10}, sf::Color{ (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255), 0}, 20, 2, 4, 6, 0.3);
                    break;
                    
                case keyMap::RESET:
                    *player = CPlayer(sf::IntRect{30, 30, 30, 30}, sf::Color{255, 255, 0, 255});
                    break;
                    
                case keyMap::TOGGLE_NOCLIP:
                    player->toggleProperty(EntityProperty::COLLIDABLE);
                    player->toggleProperty(EntityProperty::FLYING);
                    break;
                case keyMap::LOAD_ASSETS:
                    assetManager.addSpriteSheet("MAIN", "resources/gfx.png");
                    assetManager.addSpriteSheet("MAIN2", "resources/gfx2.png");
                    assetManager.addSprite("player", "MAIN2", sf::IntRect{144,396,60,164});
                    assetManager.addSprite("bush", "MAIN", sf::IntRect{160, 91, 30, 28});
                    assetManager.addSprite("tree", "MAIN", sf::IntRect{7,64,23,59});
                    assetManager.addSpriteSheet("BG", "resources/bg.png");
                    assetManager.addSprite("background", "BG", sf::IntRect{0,0,128,64});
                    assetManager.addFont("TESTFONT", "resources/font.ttf");
                    break;
                case keyMap::TOGGLE_HIDDEN:
                    player->toggleProperty(EntityProperty::HIDDEN);
                    break;
                case keyMap::TOGGLE_COLLISION_BOUNDS:
                    //player->toggleProperty(EntityProperty::FLYING);
                    entityManager.toggleRenderFlag(RenderFlags::COLLISION_BORDERS);
                    break;
                case keyMap::NEW_WINDOW:
                {
                    //player->toggleProperty(EntityProperty::STATIC);
                    window.newWindow(intro, 600, 400);
                    camera.onInit(&window);
                    assetManager.onCleanup();
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
                    player->say(text, "TESTFONT", &assetManager, &entityManager, ChatBubbleType::SAY);
                    
                    text = "";
                    for(int i = 0; i < 50; i++) {
                        text += alphanum[rand() % (sizeof(alphanum) - 1)];
                    }
                    block->say(text, "TESTFONT", &assetManager, &entityManager, ChatBubbleType::YELL);
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
                    
                default:
                    break;
                    
            }
            break;
        
        case sf::Event::KeyReleased:
            switch(event->key.code) {
                case keyMap::SNEAK:
                    player->isSneaking = false;
                    break;
                    
                default:
                    break;
            }
            break;
        default:
            break;
    }
    
}

void CGame::onLoop() {
    entityManager.onLoop();
    camera.onLoop();
}

void CGame::onRender() {
    
    sf::View view1(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    view1.setViewport(sf::FloatRect(0, 0, 1, 1));
    sf::View view2(sf::FloatRect(0.5f, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    view2.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
    view2.zoom(2.0f);
    
    window.getWindow()->setView(view1);
    window.getWindow()->clear();
    NSurface::renderRect(sf::IntRect{0,0,SCREEN_WIDTH,SCREEN_HEIGHT}, window.getWindow(), 255, 255, 255);
    entityManager.onRender(window.getWindow(), &camera);
    
    window.getWindow()->setView(view2);
    entityManager.onRender(window.getWindow(), &camera);
    
    window.getWindow()->display();
    
}

int CGame::onCleanup() {
    entityManager.onCleanup();
    assetManager.onCleanup();
    window.onCleanup();
    
    return 0;
}