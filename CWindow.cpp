//
//  CWindow.cpp
//  Third
//
//  Created by Didrik Munther on 19/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CWindow.h"
#include <iostream>

CWindow::CWindow() :
    window(nullptr), renderer(nullptr),
    screenWidth(0), screenHeight(0) {
}

int CWindow::onInit(std::string title, int width, int height, int window_flags, int renderer_flags) {
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height, window_flags);
    
    if(window == nullptr) {
        puts("e:SDL_CreateWindow Error");
        return -1;
    }
    
    renderer = SDL_CreateRenderer(window, 0, renderer_flags);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    if(renderer == nullptr) {
        puts("e:SDL_CreateRenderer Error");
        return -1;
    }
    
    screenWidth = width;
    screenHeight = height;
    
    return 0;
}

int CWindow::newWindow(std::string title, int width, int height, int window_flags, int renderer_flags) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    return onInit(title, width, height, window_flags, renderer_flags);
}

void CWindow::setTitle(std::string title) {
    SDL_SetWindowTitle(window, title.c_str());
}

int CWindow::getWidth() {
    return screenWidth;
}

int CWindow::getHeight() {
    return screenHeight;
}

SDL_Renderer* CWindow::getRenderer() {
    return renderer;
}

SDL_Window* CWindow::getWindow() {
    return window;
}

void CWindow::onCleanup() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}





