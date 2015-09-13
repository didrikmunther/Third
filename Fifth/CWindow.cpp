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

//void CWindow::updateView(int width, int height) {
//    _view = _getLetterboxView(_view, width, height);
//    _window.setView(_view);
//}
//
//void CWindow::setSize(int width, int height) {
//    _window.setSize(sf::Vector2u(width, height));
//    updateView(width, height);
//}

//sf::View CWindow::_getLetterboxView(sf::View view, int windowWidth, int windowHeight) {
//    
//    // Compares the aspect ratio of the window to the aspect ratio of the view,
//    // and sets the view's viewport accordingly in order to archieve a letterbox effect.
//    // A new view (with a new viewport set) is returned.
//    
//    float windowRatio = windowWidth / (float) windowHeight;
//    //float viewRatio = view.getSize().x / (float) view.getSize().y;
//    float viewRatio = 1980 / (float) 1080;
//    float sizeX = 1;
//    float sizeY = 1;
//    float posX = 0;
//    float posY = 0;
//    
//    bool horizontalSpacing = true;
//    if (windowRatio < viewRatio)
//        horizontalSpacing = false;
//    
//    // If horizontalSpacing is true, the black bars will appear on the left and right side.
//    // Otherwise, the black bars will appear on the top and bottom.
//    
//    if (horizontalSpacing) {
//        sizeX = viewRatio / windowRatio;
//        posX = (1 - sizeX) / 2.0;
//    }
//    
//    else {
//        sizeY = windowRatio / viewRatio;
//        posY = (1 - sizeY) / 2.0;
//    }
//    
//    view.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );
//    
//    
//    return view;
//}

void CWindow::onCleanup() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    
    _window = nullptr;
    _renderer = nullptr;
}





