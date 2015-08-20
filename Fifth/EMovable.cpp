//
//  EMovable.cpp
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EMovable.h"

EMovable::EMovable(CEntity* parent) : EComponent(parent) {
    _movementState = MovementState::WALKING_MOVEMENT;
    _movementSpeeds[MovementState::WALKING_MOVEMENT]   = 10.0f;
    _movementSpeeds[MovementState::SNEAKING_MOVEMENT] = (float)_movementSpeeds[MovementState::WALKING_MOVEMENT] / 2.0f;
    _movementSpeeds[MovementState::RUNNING_MOVEMENT]  = (float)_movementSpeeds[MovementState::WALKING_MOVEMENT] * 2.0f;
    
    jumpPower = 10.0f;
    accelerationX = 1.5f;
    accelerationY = 100.0f;
    stoppingAccelerationX = accelerationX;
    stoppingAccelerationY = accelerationY;
    hasWalkedX = false;
    hasWalkedY = false;
}

void EMovable::onLoop(CInstance* instance) {
    if(!hasWalkedX) {
        if(_parent->body.velX < 0) {
            _parent->body.velX += stoppingAccelerationX;
            if(_parent->body.velX >= 0)
                _parent->body.velX = 0.0f;
        } else {
            _parent->body.velX -= stoppingAccelerationX;
            if(_parent->body.velX <= 0)
                _parent->body.velX = 0.0f;
        }
    }
    
    if(!hasWalkedY) {
        if(_parent->hasProperty(EntityProperty::FLYING)) {
            if(_parent->body.velY < 0) {
                _parent->body.velY += stoppingAccelerationY;
                if(_parent->body.velY >= 0)
                    _parent->body.velY = 0.0f;
            } else {
                _parent->body.velY -= stoppingAccelerationY;
                if(_parent->body.velY <= 0)
                    _parent->body.velY = 0.0f;
            }
        }
    }
    
    hasWalkedX = false;
    hasWalkedY = false;
}

void EMovable::onRender(CWindow* window, CCamera* camera) {
    
}

void EMovable::renderAdditional(CWindow* window, CCamera* camera) {
    
}

bool EMovable::collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides) {
    return true;
}

void EMovable::goRight() {
    _parent->body.velX += accelerationX;
    
    if(_parent->body.velX > _movementSpeeds[_movementState])
        _parent->body.velX = _movementSpeeds[_movementState];
    
    hasWalkedX = true;
}

void EMovable::goLeft() {
    _parent->body.velX -= accelerationX;
    
    if(_parent->body.velX < -_movementSpeeds[_movementState])
        _parent->body.velX = -_movementSpeeds[_movementState];
    
    hasWalkedX = true;
}

void EMovable::goUp() {
    if(_parent->hasProperty(EntityProperty::FLYING)) {
        _parent->body.velY -= accelerationY;
        
        if(_parent->body.velY < -_movementSpeeds[_movementState])
            _parent->body.velY = -_movementSpeeds[_movementState];
    } else
        jump();
    
    hasWalkedY = true;
}

void EMovable::goDown() {
    if(_parent->hasProperty(EntityProperty::FLYING)) {
        _parent->body.velY += accelerationY;
        
        if(_parent->body.velY > _movementSpeeds[_movementState])
            _parent->body.velY = _movementSpeeds[_movementState];
    }
    
    hasWalkedY = true;
}

void EMovable::toggleNoclip() {
    _parent->toggleProperty(EntityProperty::COLLIDABLE);
    _parent->toggleProperty(EntityProperty::FLYING);
    if (_parent->hasProperty(EntityProperty::COLLIDABLE))
        _parent->setTransparency(255);
    else
        _parent->setTransparency(128);
}

void EMovable::jump() {
    if (!_parent->collisionBottom)
        return;
    
    _parent->body.velY -= accelerationY;
    if(_parent->body.velY < -jumpPower)
        _parent->body.velY = -jumpPower;
}

void EMovable::setMovementState(MovementState movementState) {
    this->_movementState = movementState;
    
    switch(movementState) {
        case MovementState::WALKING_MOVEMENT:
            _parent->setSpriteContainer(_parent->spriteStateTypes[SpriteStateTypes::WALKING]);
            break;
            
        case MovementState::SNEAKING_MOVEMENT:
            _parent->setSpriteContainer(_parent->spriteStateTypes[SpriteStateTypes::SNEAKING]);
            break;
            
        case MovementState::RUNNING_MOVEMENT:
            _parent->setSpriteContainer(_parent->spriteStateTypes[SpriteStateTypes::RUNNING]);
            break;
    }
}
