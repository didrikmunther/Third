//
//  EMovable.cpp
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EMovable.h"
#include "CEntity.h"


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
        if(parent->body.velX < 0) {
            parent->body.velX += stoppingAccelerationX;
            if(parent->body.velX >= 0)
                parent->body.velX = 0.0f;
        } else {
            parent->body.velX -= stoppingAccelerationX;
            if(parent->body.velX <= 0)
                parent->body.velX = 0.0f;
        }
    }
    
    if(!hasWalkedY) {
        if(parent->hasProperty(EntityProperty::FLYING)) {
            if(parent->body.velY < 0) {
                parent->body.velY += stoppingAccelerationY;
                if(parent->body.velY >= 0)
                    parent->body.velY = 0.0f;
            } else {
                parent->body.velY -= stoppingAccelerationY;
                if(parent->body.velY <= 0)
                    parent->body.velY = 0.0f;
            }
        }
    }
    
    hasWalkedX = false;
    hasWalkedY = false;
}

void EMovable::setMovementState(MovementState movementState) {
    //if(collisionBottom)
    this->_movementState = movementState;
    
    switch(movementState) {
        case MovementState::WALKING_MOVEMENT:
            parent->setSpriteContainer(parent->spriteStateTypes[SpriteStateTypes::WALKING]);
            break;
            
        case MovementState::SNEAKING_MOVEMENT:
            parent->setSpriteContainer(parent->spriteStateTypes[SpriteStateTypes::SNEAKING]);
            break;
            
        case MovementState::RUNNING_MOVEMENT:
            parent->setSpriteContainer(parent->spriteStateTypes[SpriteStateTypes::RUNNING]);
            break;
    }
}

void EMovable::goRight() {
    parent->body.velX += accelerationX;
    
    if(parent->body.velX > _movementSpeeds[_movementState])
        parent->body.velX = _movementSpeeds[_movementState];
    
    hasWalkedX = true;
}

void EMovable::goLeft() {
    parent->body.velX -= accelerationX;
    
    if(parent->body.velX < -_movementSpeeds[_movementState])
        parent->body.velX = -_movementSpeeds[_movementState];
    
    hasWalkedX = true;
}

void EMovable::goUp() {
    if(parent->hasProperty(EntityProperty::FLYING)) {
        parent->body.velY -= accelerationY;
        
        if(parent->body.velY < -_movementSpeeds[_movementState])
            parent->body.velY = -_movementSpeeds[_movementState];
    } else
        jump();
    
    hasWalkedY = true;
}

void EMovable::goDown() {
    if(parent->hasProperty(EntityProperty::FLYING)) {
        parent->body.velY += accelerationY;
        
        if(parent->body.velY > _movementSpeeds[_movementState])
            parent->body.velY = _movementSpeeds[_movementState];
    }
    
    hasWalkedY = true;
}

void EMovable::toggleNoclip() {
    parent->toggleProperty(EntityProperty::COLLIDABLE);
    parent->toggleProperty(EntityProperty::FLYING);
    if (parent->hasProperty(EntityProperty::COLLIDABLE))
        parent->setTransparency(255);
    else
        parent->setTransparency(128);
}

void EMovable::jump() {
    if (!parent->collisionSides.bottom)
        return;
    
    parent->body.velY -= accelerationY;
    if(parent->body.velY < -jumpPower)
        parent->body.velY = -jumpPower;
    
}

