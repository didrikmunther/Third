//
//  ELiving.cpp
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "ELiving.h"
#include "CCombatText.h"
#include "CEntity.h"
#include "CInstance.h"

ELiving::ELiving(CEntity* parent) : EComponent(parent) {
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

void ELiving::onLoop(CInstance* instance) {
    
}

void ELiving::onRender(CWindow* window, CCamera* camera) {
    
}

void ELiving::renderAdditional(CWindow* window, CCamera* camera) {
    if (_parent->isDead || _parent->hasProperty(EntityProperty::HIDDEN))
        return;
    
    int floatOverHead = 10;
    int bgWidth = 100;
    int bgHeight = 15;
    int healthWidth = ((float)_values[ValueTypes::HEALTH] / _maxValues[ValueTypes::HEALTH]) * bgWidth;
    int kevlarWidth = ((float)_values[ValueTypes::KEVLAR] / _maxValues[ValueTypes::KEVLAR]) * bgWidth;
    
    if(camera->collision(_parent->body.getX() + _parent->body.getW() / 2 - bgWidth / 2,
                         _parent->body.getY() - bgHeight - floatOverHead,
                         bgWidth,
                         bgHeight)) {
        
        NSurface::renderRect(_parent->body.getX() + _parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Background
                             _parent->body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             bgWidth,
                             bgHeight,
                             window, 255, 0, 0);
        NSurface::renderRect(_parent->body.getX() + _parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Health
                             _parent->body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             healthWidth,
                             bgHeight,
                             window, 0, 255, 0);
        NSurface::renderRect(_parent->body.getX() + _parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Kevlar
                             _parent->body.getY() - bgHeight - floatOverHead - camera->offsetY() + bgHeight / 2 + 1,
                             kevlarWidth,
                             bgHeight / 2,
                             window, 128, 128, 128);
    }
}

bool ELiving::collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides) {
    float jumpDamageHeight = 16;
    if(_parent->body.velY > jumpDamageHeight && collisionSides.collisionBottom) {            // Fall damage
        //dealDamage((_parent->body.velY - jumpDamageHeight) * (_maxValues[ValueTypes::HEALTH] / (GRAVITY * 166)));
        dealDamage((_parent->body.velY - jumpDamageHeight) * _maxValues[ValueTypes::HEALTH] / 10);
        if(_parent == instance->player.get())
            instance->camera.addCameraShake((_parent->body.velY - 16) * FALL_DAMAGE_CAMERA_SHAKE_INTENSIFIER);
    }
    
    return true;
}

int ELiving::dealDamage(int amount, UtilityPosition position /* = {0, 0} */, CEntity* damager /* = nullptr */ ) {
    if(position.x == 0)
        position.x = _parent->body.getX();
    if(position.y == 0)
        position.y = _parent->body.getY();
    
    SDL_Color damageColor = {255, 0, 0};
    
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
        _parent->isDead = true;
    }
    
    //auto text = std::make_shared<CCombatText>(position.x, position.y, damageColor, 20, "-" + std::to_string(damageDone), "TESTFONT");
    //_parent->addGuiText(text);
    
    return damageDone;
}

int ELiving::heal(int amount, UtilityPosition position /* = {0, 0} */, CEntity* healer /* = nullptr */ ) {
    if(position.x == 0)
        position.x = _parent->body.getX();
    if(position.y == 0)
        position.y = _parent->body.getY();
    
    SDL_Color healingColor = {0, 255, 0};
    
    int* health = &_values[ValueTypes::HEALTH];
    *health += amount;
    int overHeal = 0;
    if(*health >= _maxValues[ValueTypes::HEALTH]) {
        overHeal = *health - _maxValues[ValueTypes::HEALTH];
        *health = _maxValues[ValueTypes::HEALTH];
    }
    int healed = amount - overHeal;
    
    //auto text = std::make_shared<CCombatText>(position.x, position.y, healingColor, 20, "+" + std::to_string(healed), "TESTFONT");
    //_parent->addGuiText(text);
    
    return healed;
}
