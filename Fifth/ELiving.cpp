//
//  ELiving.cpp
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <algorithm>

#include "ELiving.h"
#include "CCamera.h"
#include "CWindow.h"
#include "CEntity.h"
#include "NSurface.h"
#include "CCombatText.h"
#include "CGlobalSettings.h"

#include "EMovable.h"


ELiving::ELiving(CEntity* parent) : EComponent(parent) {
    _delay = 1000; // Ms
    _timer = SDL_GetTicks();
    std::fill(_bufferedValues, _bufferedValues+ValueTypes::VALUETYPES_TOTAL, 0);
    std::fill(_animatedBufferedValues, _animatedBufferedValues+ValueTypes::VALUETYPES_TOTAL, 0);
    std::fill(_animatedBufferedIncrements, _animatedBufferedIncrements+ValueTypes::VALUETYPES_TOTAL, 0);
    
    _hasBeenDamaged     =
    _hasBeenHealed      =
    _hasTakenFallDamage = false;
    
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
    _animatedBufferedValues[ValueTypes::KEVLAR] -= _animatedBufferedValues[ValueTypes::KEVLAR] > 0 ?_animatedBufferedIncrements[ValueTypes::KEVLAR] : _animatedBufferedValues[ValueTypes::KEVLAR]; // If negative value set to 0
    if(_animatedBufferedValues[ValueTypes::KEVLAR] == 0) { // Don't increment hp until kevlar is animated
        _animatedBufferedValues[ValueTypes::HEALTH] -= _animatedBufferedValues[ValueTypes::HEALTH] > 0 ?_animatedBufferedIncrements[ValueTypes::HEALTH] : _animatedBufferedValues[ValueTypes::HEALTH];
    }
    
    if(_hasBeenDamaged || _hasBeenHealed) {
        _timer = SDL_GetTicks();
    }
    
    if(SDL_GetTicks() - _timer > _delay
       && _animatedBufferedValues[ValueTypes::HEALTH] == 0
       && _animatedBufferedValues[ValueTypes::KEVLAR] == 0) {
        _timer += _delay;
        
        _animatedBufferedValues[ValueTypes::HEALTH] = _bufferedValues[ValueTypes::HEALTH];
        _animatedBufferedValues[ValueTypes::KEVLAR] = _bufferedValues[ValueTypes::KEVLAR];
        
        //        _animatedBufferedIncrements[ValueTypes::HEALTH] = _animatedBufferedValues[ValueTypes::HEALTH] /(1000.0f / GAMEINTERVAL);
        //        _animatedBufferedIncrements[ValueTypes::KEVLAR] = _animatedBufferedValues[ValueTypes::KEVLAR] /(1000.0f / GAMEINTERVAL); // Make the animation happen in exactly one second
        
        _animatedBufferedIncrements[ValueTypes::HEALTH] =
        _animatedBufferedIncrements[ValueTypes::KEVLAR] = 10;
        
        _bufferedValues[ValueTypes::HEALTH] =
        _bufferedValues[ValueTypes::KEVLAR] = 0;
    }
    
    _hasBeenDamaged =
    _hasBeenHealed  = false;
}

void ELiving::onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    if (parent->isDead() || parent->hasProperty(EntityProperty::HIDDEN))
        return;
    
    int floatOverHead = 10;
    int bgWidth = 100;
    int bgHeight = 15;
    float healthWidth = ((float)_values[ValueTypes::HEALTH] / _maxValues[ValueTypes::HEALTH]) * bgWidth;
    float kevlarWidth = ((float)_values[ValueTypes::KEVLAR] / _maxValues[ValueTypes::KEVLAR]) * bgWidth;
    
    float bufferedHealthWidth = healthWidth + (((float)_bufferedValues[ValueTypes::HEALTH] / _maxValues[ValueTypes::HEALTH]) * bgWidth); // healthWidth + bufferedHealth / maxHealth
    float bufferedKevlarWidth = kevlarWidth + (((float)_bufferedValues[ValueTypes::KEVLAR] / _maxValues[ValueTypes::KEVLAR]) * bgWidth); // kevlarWidth + bufferedKevlar / maxKevlar
    
    float bufferedHealthAnimated = bufferedHealthWidth + (((float)_animatedBufferedValues[ValueTypes::HEALTH] / _maxValues[ValueTypes::HEALTH]) * bgWidth); // bufferedHealthWidth + animatedBufferedHealth / maxHealth
    float bufferedKevlarAnimated = bufferedKevlarWidth + (((float)_animatedBufferedValues[ValueTypes::KEVLAR] / _maxValues[ValueTypes::KEVLAR]) * bgWidth); // bufferedKevlarWidth + animatedBufferedKevlar / maxHealth
    
    if(camera->collision(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2,
                         parent->body.getY() - bgHeight - floatOverHead,
                         bgWidth,
                         bgHeight)) {
        
        NSurface::renderRect(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Background
                             parent->body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             bgWidth,
                             bgHeight,
                             window, 255, 0, 0);
        NSurface::renderRect(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Buffered h
                             parent->body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             (int)ceil(bufferedHealthWidth),
                             bgHeight,
                             window, 234, 245, 166);
        NSurface::renderRect(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Animated h
                             parent->body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             (int)ceil(bufferedHealthAnimated),
                             bgHeight,
                             window, 235, 245, 166);
        NSurface::renderRect(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Health
                             parent->body.getY() - bgHeight - floatOverHead - camera->offsetY(),
                             (int)ceil(healthWidth),
                             bgHeight,
                             window, 0, 255, 0);
        NSurface::renderRect(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Buffered k
                             parent->body.getY() - bgHeight - floatOverHead - camera->offsetY() + bgHeight / 2 + 1,
                             (int)ceil(bufferedKevlarWidth),
                             bgHeight / 2,
                             window, 200, 128, 255);
        NSurface::renderRect(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Animated k
                             parent->body.getY() - bgHeight - floatOverHead - camera->offsetY() + bgHeight / 2 + 1,
                             (int)ceil(bufferedKevlarAnimated),
                             bgHeight / 2,
                             window, 200, 128, 255);
        NSurface::renderRect(parent->body.getX() + parent->body.getW() / 2 - bgWidth / 2 - camera->offsetX(),  // Kevlar
                             parent->body.getY() - bgHeight - floatOverHead - camera->offsetY() + bgHeight / 2 + 1,
                             (int)ceil(kevlarWidth),
                             bgHeight / 2,
                             window, 128, 128, 128);
    }
}

bool ELiving::onCollision(CEntity* target, CollisionSides* collisionSides) {
    
    float jumpPower = 10.0f;
    EMovable* movable = parent->getComponent<EMovable>();
    if(movable)
        jumpPower = movable->jumpPower;
    
    _hasTakenFallDamage = false;
    float jumpDamageHeight = jumpPower + jumpPower / 4;
    if(parent->body.velY > jumpDamageHeight && collisionSides->bottom && !_hasTakenFallDamage) {            // Fall damage
        dealDamage((parent->body.velY - jumpDamageHeight) * (_maxValues[ValueTypes::HEALTH] / (CGlobalSettings::GRAVITY * 166))); // At 0.3 gravity the lethal velocity is 50
        _hasTakenFallDamage = true;
    }
    
    return true;
}

int ELiving::dealDamage(int amount, UtilityPosition position /* = {0, 0} */, CEntity* damager /* = nullptr */) {
    SDL_Color damageColor = {255, 0, 0};
    
    if(position.x == 0)
        position.x = parent->body.getX();
    if(position.y == 0)
        position.y = parent->body.getY();
    
    int* health = &_values[ValueTypes::HEALTH];
    int* kevlar = &_values[ValueTypes::KEVLAR];
    
    int afterKevlar = 0;
    
    *kevlar -= amount;
    if(*kevlar <= 0) {
        afterKevlar = -*kevlar;
        *kevlar = 0;
    }
    int overDamage = *health - afterKevlar <= 0 ? amount - (*health - afterKevlar) : -1;
    int damageDone = amount - overDamage > 0 ? amount - overDamage : 0;
    
    *health -= afterKevlar;
    if(*health <= 0) {
        *health = 0;
        *isDead() = true;
    }
    
    _bufferedValues[ValueTypes::KEVLAR] += amount - afterKevlar;
    _bufferedValues[ValueTypes::HEALTH] += _values[ValueTypes::HEALTH] > _bufferedValues[ValueTypes::HEALTH] + _values[ValueTypes::HEALTH] ? -_bufferedValues[ValueTypes::HEALTH] : afterKevlar; // If health is overlapping the buffered damage, set the buffered damage to 0 by adding it to (-)itself.
    
    CCombatText* text = new CCombatText(position.x, position.y, damageColor, 20, "-" + std::to_string(damageDone), "TESTFONT");
    guiTextVector()->push_back(text);
    
    _hasBeenDamaged = true;
    
    return damageDone;
}

int ELiving::heal(int amount, UtilityPosition position /* = {0, 0} */, CEntity* healer /* = nullptr */) {
    if(position.x == 0)
        position.x = parent->body.getX();
    if(position.y == 0)
        position.y = parent->body.getY();
    
    SDL_Color healingColor = {0, 255, 0};
    
    int* health = &_values[ValueTypes::HEALTH];
    *health += amount;
    int overHeal = 0;
    if(*health >= _maxValues[ValueTypes::HEALTH]) {
        overHeal = *health - _maxValues[ValueTypes::HEALTH];
        *health = _maxValues[ValueTypes::HEALTH];
    }
    int healed = amount - overHeal;
    
    _bufferedValues[ValueTypes::HEALTH] -= healed;
    
    CCombatText* text = new CCombatText(position.x, position.y, healingColor, 20, "+" + std::to_string(healed), "TESTFONT");
    guiTextVector()->push_back(text);
    
    _hasBeenHealed = true;
    
    return healed;
}


