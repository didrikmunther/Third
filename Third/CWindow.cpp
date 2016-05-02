//
//  CWindow.cpp
//  Third
//
//  Created by Didrik Munther on 19/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CWindow.h"
#include "NFile.h"


CWindow::CWindow() :
    _screenWidth(0), _screenHeight(0) {
}

int CWindow::onInit(std::string title, int width, int height) {
    
    Uint32 window_flags = SDL_WINDOW_SHOWN;
    Uint32 renderer_flags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_SOFTWARE;
    
    _window = SDL_CreateWindow(title.c_str(), 0, 0,
                              width, height, window_flags);
    
    if(_window == nullptr) {
        NFile::log(LogType::ERROR, "At SDL_CreateWindow: ", SDL_GetError());
        return -1;
    }
    
    _renderer = SDL_CreateRenderer(_window, 0, renderer_flags);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
    
    if(_renderer == nullptr) {
        NFile::log(LogType::ERROR, "At SDL_CreateRenderer: ", SDL_GetError());
        return -1;
    }
    
    _screenWidth = width;
    _screenHeight = height;
    
    return 0;
}

int CWindow::newWindow(std::string title, int width, int height) {
    onCleanup();
    return onInit(title, width, height);
}

void CWindow::setTitle(std::string title) {
    SDL_SetWindowTitle(_window, title.c_str());
}

int CWindow::getWidth() {
    return _screenWidth;
}

int CWindow::getHeight() {
    return _screenHeight;
}

SDL_Window* CWindow::getWindow() {
    return _window;
}

SDL_Renderer* CWindow::getRenderer() {
    return _renderer;
}

void CWindow::onCleanup() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    
    _window = nullptr;
    _renderer = nullptr;
}





