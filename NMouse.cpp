//
//  NMouse.cpp
//  Third
//
//  Created by Didrik Munther on 16/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NMouse.h"
#include <SFML/Graphics.hpp>

int NMouse::absoluteMouseX() {
    sf::Vector2i position = sf::Mouse::getPosition();
    return position.x;
}

int NMouse::absoluteMouseY() {
    sf::Vector2i position = sf::Mouse::getPosition();
    return position.y;
}

int NMouse::relativeMouseX(CCamera* camera) {
    sf::Vector2i position = sf::Mouse::getPosition();
    return position.x + camera->offsetX();
}

int NMouse::relativeMouseY(CCamera* camera) {
    sf::Vector2i position = sf::Mouse::getPosition();
    return position.y + camera->offsetY();
}