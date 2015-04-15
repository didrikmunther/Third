//
//  CGame.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGame.h"
#include <iostream>
#include "CSurface.h"
#include <cmath>
#include "Define.h"

CGame::CGame() :
running(true), window(nullptr), intro("Physics"),
WIDTH(640), HEIGHT(480), BPP(32), camera(WIDTH, HEIGHT),
//WIDTH(1280), HEIGHT(720), BPP(32),
lastTime(SDL_GetTicks()), timer(SDL_GetTicks()),
ns(1000.0f / (float)GAMEINTERVAL), delta(0), frames(0), updates(0),
mouseX(0), mouseY(0) {
}

CGame::~CGame() {
}

int CGame::onExecute() {
    
    std::cout << "Initializing game...\n";
    
    switch(onInit()){
        case -1:
            std::cout << "Initializing failed!\n";
            return 1;
            break;
        case 0:
            std::cout << "Initializing succesful!\n";
            break;
    }
    
    std::cout << "Starting game...\n";
    
    while(running) {
        while(SDL_PollEvent(&event)){
            onEvent(&event);
        }
        
        handleKeyStates();
        
        std::cout << "X: " << player->body.velX << ", Y: " << player->body.velY << " \n";
        
        float now = SDL_GetTicks();
        delta += (now - lastTime) / ns;
        lastTime = now;
        
        while(delta >= 1) {
            std::cout << (int)floor(delta) << " | ";
            if(delta > 20) {       // To make sure it doesn't freeze
                entityManager.particleCleanup();
            }
            
            onLoop();
            
            updates++;
            delta--;
        }
        
        onRender();
        
        frames++;
        
        if(SDL_GetTicks() - timer > 1000) {
            timer += 1000;
            title.str("");
            title << intro << " | " << updates << " ups, " << frames << " fps";
            SDL_SetWindowTitle(window, title.str().c_str());
            updates = 0;
            frames = 0;
        }
        
        //SDL_Delay(7);
        
    }
    
    std::cout << "Ending game...\n";
    
    return onCleanup();
    
}

int CGame::onInit() {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        puts("SDL_Init error");
        return -1;
    }
    
    window = SDL_CreateWindow(intro.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WIDTH, HEIGHT, SCREEN_FLAGS);
    
    if(window == nullptr) {
        puts("SDL_CreateWindow Error");
        return -1;
    }
    
    renderer = SDL_CreateRenderer(window, 0, RENDERER_FLAGS);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    player = new CPlayer(SDL_Rect{30, 30, 30, 30}, SDL_Color{255, 255, 0, 255});
    entityManager.addEntity(player);
    camera.setTarget(player);
    
    entityManager.addEntity(SDL_Rect{0 - 30 / 2, 480 - 30 / 2, 5000, 30}, SDL_Color{255, 0, 0, 0});
    entityManager.addEntity(SDL_Rect{0 - 30 / 2, 480 - 500, 30, 500}, SDL_Color{255, 0, 0, 0});
    entityManager.addEntity(SDL_Rect{0 + 500, 480 - 500, 30, 500}, SDL_Color{255, 0, 0, 0});

    return 0;
}

void CGame::handleKeyStates() {
    
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    bool keyPressed = false;              // To stop movement if no keys are pressed
    if(keystate[SDL_SCANCODE_D]) {
        player->body.velX += player->accelerationX;
        if(player->body.velX > player->maxSpeed)
            player->body.velX = player->maxSpeed;
        keyPressed = true;
    }
    if(keystate[SDL_SCANCODE_A]) {
        player->body.velX -= player->accelerationX;
        if(player->body.velX < -player->maxSpeed)
            player->body.velX = -player->maxSpeed;
        keyPressed = true;
    }
    if(keystate[SDL_SCANCODE_W]) {
        player->body.velY -= player->accelerationY;
        if(player->body.velY < -player->maxSpeed)
            player->body.velY = -player->maxSpeed;
        keyPressed = true;
    }
    if(keystate[SDL_SCANCODE_S]) {
        player->body.velY += player->accelerationY;
        if(player->body.velY > player->maxSpeed)
            player->body.velY = player->maxSpeed;
        keyPressed = true;
    }
    
    if(!keyPressed) {
        CBody* body = &player->body;
        if(body->velX < 0) {
            std::cout << "here\n";
            body->velX += player->stoppingAccelerationX;
            if(body->velX >= 0)
                body->velX = 0.0f;
        } else {
            body->velX -= player->stoppingAccelerationX;
            if(body->velX <= 0)
                body->velX = 0.0f;
        }
        
//        if(body->velY < 0) {
//            body->velY += player->accelerationY;
//            if(body->velY >= 0)
//                body->velY = 0.0f;
//        } else {
//            body->velY -= player->accelerationY;
//            if(body->velY <= 0)
//                body->velY = 0.0f;
//        }
    }
    
}

void CGame::onEvent(SDL_Event* event) {
    
    
    
    //if(event->key.repeat != 0) return;
    
    switch(event->type) {
        case SDL_QUIT:
            running = false;
            break;
            
        case SDL_KEYDOWN:
            switch(event->key.keysym.sym) {
                    
                case keyMap::EXIT:
                    running = false;
                    break;
                    
                case keyMap::BLOCK:
                    //entityManager.onCleanup();
                    entityManager.addEntity(SDL_Rect{mouseX - 30 / 2, mouseY - 30 / 2, 40, 40}, SDL_Color{255, 0, 0, 0});

                    break;
                    
                case keyMap::PARTICLE:
                    entityManager.addParticle(SDL_Rect{mouseX - 20 / 2, mouseY - 20 / 2, 20, 20}, SDL_Color{(Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0}, 5);
                    break;
                    
                case keyMap::PARTICLEEM:
                    entityManager.addParticleEmitter(SDL_Rect{mouseX - 4 / 2, mouseY - 4 / 2, 10, 10}, SDL_Color{ (Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0}, 20, 2, 6, 0.3);
                    break;
                    
                case keyMap::RESET:
                    *player = CPlayer(SDL_Rect{30, 30, 30, 30}, SDL_Color{255, 255, 0, 255});
                    break;
            }
            break;
        
        case SDL_KEYUP:
            switch(event->key.keysym.sym) {
                    
            }
            break;
        
        case SDL_MOUSEMOTION:
            mouseX = event->motion.x + camera.offsetX();
            mouseY = event->motion.y + camera.offsetY();
            break;
    }
    
}

void CGame::onLoop() {
    entityManager.onLoop();
}

void CGame::onRender() {
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    entityManager.onRender(renderer, &camera);
    
    SDL_RenderPresent(renderer);
    
}

int CGame::onCleanup() {
    entityManager.onCleanup();
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}