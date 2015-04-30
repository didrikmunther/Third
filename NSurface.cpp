//
//  NSurface.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NSurface.h"

void NSurface::renderRect(int x, int y, int w, int h, sf::RenderWindow* window, int r, int g, int b) {
    renderRect(sf::IntRect{x,y,w,h}, window, r, g, b);
}

void NSurface::renderRect(sf::IntRect rect, sf::RenderWindow* window, int r, int g, int b) {
    sf::RectangleShape rectangle(sf::Vector2f(rect.left, rect.top));
    rectangle.setPosition(rect.width, rect.height);
    rectangle.setFillColor(sf::Color(r, g, b));
    window->draw(rectangle);
}

void NSurface::renderSprite(CSprite* sprite, sf::RenderWindow* window, sf::IntRect destination) {
    sprite->setPosition(destination.left, destination.top);
    window->draw(*sprite->getSprite());
}

//void NSurface::renderSprite(CSprite* sprite, SDL_Renderer* renderer, SDL_Rect destination, double angle, SDL_Point* center, SDL_RendererFlip flip) {
//    SDL_Rect src{sprite->getSource()->x, sprite->getSource()->y, sprite->getSource()->w, sprite->getSource()->h};
//    SDL_RenderCopyEx(renderer, sprite->getSpriteSheet()->getTexture(), &src, &destination, angle, center, flip);
//}

void NSurface::renderText(int x, int y, CText* textObj, sf::RenderWindow* window) {
    sf::Text text(textObj->getText()->c_str(), *textObj->getFont(), 10);
    window->draw(text);
}


