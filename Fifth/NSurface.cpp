//
//  NSurface.cpp
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "NSurface.h"
#include "Define.h"
#include "CSpriteContainer.h"
#include <iostream>

void NSurface::renderRect(int x, int y, int w, int h, sf::RenderTarget& target, int r, int g, int b, int a) {
    renderRect(sf::IntRect{x,y,w,h}, target, r, g, b, a);
}

void NSurface::renderRect(sf::IntRect rect, sf::RenderTarget& target, int r, int g, int b, int a) {
    sf::RectangleShape rectangle(sf::Vector2f(rect.width, rect.height));
    rectangle.setPosition(rect.left, rect.top);
    rectangle.setFillColor(sf::Color(r, g, b, a));
    
    target.draw(rectangle);
}

void NSurface::renderEntity(CEntity* entity, CWindow* window, sf::IntRect destination) {
    if(entity->getSpriteContainer()->getSprite() == nullptr) return;
    auto nsprite = *entity->getSpriteContainer()->getSprite()->getSprite();
    nsprite.setPosition(destination.left, destination.top);
    nsprite.setColor(sf::Color{255, 255, 255, (uint8_t)entity->getTransparency()});
    
    int spriteWidth, spriteHeight = 0;
    if(entity->spriteFollowsCollisionBox) {
        spriteWidth = entity->body.getW();
        spriteHeight = entity->body.getH();
    } else {
        spriteWidth = entity->getSpriteContainer()->spriteArea.w;
        spriteHeight = entity->getSpriteContainer()->spriteArea.h;
    }
    
    if(entity->hasProperty(EntityProperty::FLIP)) {
        nsprite.setOrigin({ nsprite.getLocalBounds().width, 0 });
        nsprite.setScale(-spriteWidth / entity->getSpriteContainer()->getSprite()->getSprite()->getGlobalBounds().width, spriteHeight / entity->getSpriteContainer()->getSprite()->getSprite()->getGlobalBounds().height);
    } else {
        nsprite.setScale(spriteWidth / entity->getSpriteContainer()->getSprite()->getSprite()->getGlobalBounds().width, spriteHeight / entity->getSpriteContainer()->getSprite()->getSprite()->getGlobalBounds().height);
    }
    
    window->getRenderTexture()->draw(nsprite);
    
//    sf::Shader* shader = CAssetManager::getShader(entity->getShaderKey());
//    sf::RenderStates states;
//    if(shader != nullptr) {
//        shader->setParameter("frag_LightOrigin", sf::Vector2f(destination.left + destination.width / 2, destination.top + destination.height / 2));
//        shader->setParameter("frag_LightColor", sf::Vector3f(0, 255, 0));
//        shader->setParameter("frag_LightAttenuation", 10);
//        shader->setParameter("frag_ScreenResolution", sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
//        states.shader = shader;
//        states.blendMode = sf::BlendAdd;
//    }
    
    
    //window->getRenderTexture()->draw(*window->getSprite(), states);
    window->getRenderTexture()->draw(*window->getSprite());
    
}

void NSurface::renderText(int x, int y, CText* textObj, sf::RenderTarget& target) {
    sf::Text text(textObj->getText()->c_str(), *textObj->getFont(), textObj->getSize());
    text.setColor(*textObj->getColor());
    text.setPosition(x, y);
    
    target.draw(text);
}


