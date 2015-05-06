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
    _screenWidth(0), _screenHeight(0) {
}

int CWindow::onInit(std::string title, int width, int height) {
    
    _window.create(sf::VideoMode(width, height), sf::String(title));
    _window.setVerticalSyncEnabled(true);
    
    _screenWidth = width;
    _screenHeight = height;
    
    return 0;
}

int CWindow::newWindow(std::string title, int width, int height) {

    onCleanup();
    return onInit(title, width, height);
    
}

void CWindow::setTitle(std::string title) {
    _window.setTitle(sf::String(title));
}

int CWindow::getWidth() {
    return _screenWidth;
}

int CWindow::getHeight() {
    return _screenHeight;
}

sf::RenderWindow* CWindow::getWindow() {
    return &_window;
}

sf::RenderTexture* CWindow::getRenderTexture() {
    return &_renderTexture;
}

sf::Sprite* CWindow::getSprite() {
    return &_sprite;
}

void CWindow::onCleanup() {
    
}





