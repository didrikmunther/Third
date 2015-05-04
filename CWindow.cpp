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
    screenWidth(0), screenHeight(0) {
}

int CWindow::onInit(std::string title, int width, int height) {
    
    window.create(sf::VideoMode(width, height), sf::String(title));
    window.setVerticalSyncEnabled(true);
    
    screenWidth = width;
    screenHeight = height;
    
    return 0;
}

int CWindow::newWindow(std::string title, int width, int height) {

//    sf::RenderWindow tempWindow(sf::VideoMode(width, height), sf::String(title));
//    
//    window = tempWindow;
//    
//    return onInit(title, width, height, window_flags, renderer_flags);
    
    return 0;
    
}

void CWindow::setTitle(std::string title) {
    window.setTitle(sf::String(title));
}

int CWindow::getWidth() {
    return screenWidth;
}

int CWindow::getHeight() {
    return screenHeight;
}


sf::RenderWindow* CWindow::getWindow() {
    return &window;
}

void CWindow::onCleanup() {
    
}





