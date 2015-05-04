//
//  NSurface.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NSurface.h"
#include "Define.h"
#include <iostream>

void NSurface::renderRect(int x, int y, int w, int h, sf::RenderWindow* window, int r, int g, int b) {
    renderRect(sf::IntRect{x,y,w,h}, window, r, g, b);
}

void NSurface::renderRect(sf::IntRect rect, sf::RenderWindow* window, int r, int g, int b) {
    sf::RectangleShape rectangle(sf::Vector2f(rect.width, rect.height));
    rectangle.setPosition(rect.left, rect.top);
    rectangle.setFillColor(sf::Color(r, g, b));
    window->draw(rectangle);
}

void NSurface::renderSprite(CSprite* sprite, sf::RenderWindow* window, sf::IntRect destination) {
//    auto nsprite = *sprite->getSprite();
//    nsprite.setPosition(destination.left, destination.top);
//    nsprite.setScale(destination.width / sprite->getSprite()->getGlobalBounds().width, destination.height / sprite->getSprite()->getGlobalBounds().height);
//    window->draw(nsprite);
    
    sf::RectangleShape rectangle(sf::Vector2f(destination.width, destination.height));
    rectangle.setTexture(sprite->getSprite()->getTexture());
    rectangle.setPosition(destination.left, destination.top);
    rectangle.setTextureRect(sf::IntRect(sprite->getOffset()->left,
                                         sprite->getOffset()->top,
                                         sprite->getOffset()->width,
                                         sprite->getOffset()->height));
    window->draw(rectangle);
    
}

void NSurface::renderText(int x, int y, CText* textObj, sf::RenderWindow* window) {
    sf::Text text(textObj->getText()->c_str(), *textObj->getFont(), textObj->getSize());
    text.setColor(*textObj->getColor());
    text.setPosition(x, y);
    window->draw(text);
}


