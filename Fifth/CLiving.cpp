//
//  CLiving.cpp
//  Fifth
//
//  Created by Didrik Munther on 01/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CLiving.h"
#include "NSurface.h"
#include "CCombatText.h"

CLiving::CLiving(Box rect, sf::Color color) : CMovable(rect, color) {
    _init();
}

CLiving::CLiving(Box rect, std::string spriteKey) : CMovable(rect, spriteKey) {
    _init();
}

void CLiving::_init() {
    entityType = EntityTypes::Living;
    
    _values[ValueTypes::HEALTH]      = _maxValues[ValueTypes::HEALTH] = 1000;
    _values[ValueTypes::KEVLAR]      = _maxValues[ValueTypes::KEVLAR] = 1000;
    _values[ValueTypes::ENERGY]      = _maxValues[ValueTypes::ENERGY] = 100;
    _values[ValueTypes::STAMINA]     = _maxValues[ValueTypes::STAMINA] = 100;
    _stats [StatTypes::ARMOUR]       = 1;
    _stats [StatTypes::ATTACK_POWER] = 10;
    _stats [StatTypes::ATTACK_SPEED] = 10;

    // temp
    _values[ValueTypes::HEALTH] = 750;
    _values[ValueTypes::KEVLAR] = 500;
}

void CLiving::renderAdditional(CWindow* window, CCamera* camera, int renderFlags) {
    CMovable::renderAdditional(window, camera, renderFlags);
    
    if (isDead())
        return;
    
    int floatOverHead = 10;
    int bgWidth = 100;
    int bgHeight = 15;
    int healthWidth = ((float)_values[ValueTypes::HEALTH] / _maxValues[ValueTypes::HEALTH]) * bgWidth;
    int kevlarWidth = ((float)_values[ValueTypes::KEVLAR] / _maxValues[ValueTypes::KEVLAR]) * bgWidth;
    
    if(camera->collision(body.getX() + body.getW() / 2 - bgWidth / 2,
                         body.getY() - bgHeight - floatOverHead,
                         bgWidth,
                         bgHeight)) {
        
        NSurface::renderRect(body.getX() + body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Background
                             body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             bgWidth,
                             bgHeight,
                             *window->getRenderTexture(), 255, 0, 0);
        NSurface::renderRect(body.getX() + body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Health
                             body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             healthWidth,
                             bgHeight,
                             *window->getRenderTexture(), 0, 255, 0);
        NSurface::renderRect(body.getX() + body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Kevlar
                             body.getY() - bgHeight - floatOverHead - camera->offsetY() + bgHeight / 2 + 1,
                             kevlarWidth,
                             bgHeight / 2,
                             *window->getRenderTexture(), 128, 128, 128);
    }
    
}

int CLiving::dealDamage(int amount, UtilityPosition position) {
    
    if(position.x == 0)
        position.x = body.getX();
    if(position.y == 0)
        position.y = body.getY();
    
    sf::Color damageColor = {255, 0, 0};
    
    int* health = &_values[ValueTypes::HEALTH];
    int* kevlar = &_values[ValueTypes::KEVLAR];
    
    int afterKevlar = 0;
    
    *kevlar -= amount;
    if(*kevlar <= 0) {
        afterKevlar = -*kevlar;
        *kevlar = 0;
    }
    int overDamage = (*health-afterKevlar <= 0?amount-(*health-afterKevlar):0);
    int damageDone = amount - overDamage;
    *health -= afterKevlar;
    if(*health <= 0) {
        *health = 0;
        _isDead = true;
    }
    
    CCombatText* text = new CCombatText(position.x, position.y, damageColor, 20, "-" + std::to_string(damageDone), "TESTFONT");
    _GuiTextVector.push_back(text);
    
    return damageDone;
}

int CLiving::heal(int amount, UtilityPosition position) {
    
    if(position.x == 0)
        position.x = body.getX();
    if(position.y == 0)
        position.y = body.getY();
    
    sf::Color healingColor = {0, 255, 0};
    
    int* health = &_values[ValueTypes::HEALTH];
    *health += amount;
    int overHeal = 0;
    if(*health >= _maxValues[ValueTypes::HEALTH]) {
        overHeal = *health - _maxValues[ValueTypes::HEALTH];
        *health = _maxValues[ValueTypes::HEALTH];
    }
    int healed = amount - overHeal;
    
    CCombatText* text = new CCombatText(position.x, position.y, healingColor, 20, "+" + std::to_string(healed), "TESTFONT");
    _GuiTextVector.push_back(text);
    
    return healed;
}

void CLiving::_doLogic() {
    CMovable::_doLogic();
    
}

bool CLiving::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CMovable::_collisionLogic(target, collisionSides);
    bool collision = true;
    
    if(body.velY > 22 && collisionBottom) {            // Fall damage
        dealDamage(10 * (body.velY - 22) * (_maxValues[ValueTypes::HEALTH] / 500));
    }
    
    return parentCollision && collision;
}

