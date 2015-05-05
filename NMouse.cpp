//
//  NMouse.cpp
//  Third
//
//  Created by Didrik Munther on 16/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NMouse.h"
#include <iostream>

int NMouse::absoluteMouseX(sf::RenderWindow* window) {
    sf::Vector2i position = sf::Mouse::getPosition(*window);
    return position.x;
}

int NMouse::absoluteMouseY(sf::RenderWindow* window) {
    sf::Vector2i position = sf::Mouse::getPosition(*window);
    return position.y;
}

int NMouse::relativeMouseX(sf::RenderWindow* window, CCamera* camera) {
    sf::Vector2i position = sf::Mouse::getPosition(*window);
    return position.x + camera->offsetX();
}

int NMouse::relativeMouseY(sf::RenderWindow* window, CCamera* camera) {
    sf::Vector2i position = sf::Mouse::getPosition(*window);
    return position.y + camera->offsetY();
}