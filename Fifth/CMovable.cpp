//
//  CMovable.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CMovable.h"


CMovable::CMovable(Box rect, SDL_Color color) :
    CEntity(rect, color) {
    _init();
}

CMovable::CMovable(Box rect, std::string spriteKey) :
    CEntity(rect, spriteKey) {
    _init();
}

void CMovable::_init() {
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

void CMovable::renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    CEntity::renderAdditional(window, camera, renderFlags);
    
}

void CMovable::setMovementState(MovementState movementState) {
    //if(collisionBottom)
    this->_movementState = movementState;
    
    switch(movementState) {
        case MovementState::WALKING_MOVEMENT:
            setSpriteContainer(spriteStateTypes[SpriteStateTypes::WALKING]);
            break;
            
        case MovementState::SNEAKING_MOVEMENT:
            setSpriteContainer(spriteStateTypes[SpriteStateTypes::SNEAKING]);
            break;
            
        case MovementState::RUNNING_MOVEMENT:
            setSpriteContainer(spriteStateTypes[SpriteStateTypes::RUNNING]);
            break;
    }
}

void CMovable::goRight() {
    body.velX += accelerationX;
    
    if(body.velX > _movementSpeeds[_movementState])
        body.velX = _movementSpeeds[_movementState];
    
    hasWalkedX = true;
}

void CMovable::goLeft() {
    body.velX -= accelerationX;
    
    if(body.velX < -_movementSpeeds[_movementState])
        body.velX = -_movementSpeeds[_movementState];
    
    hasWalkedX = true;
}

void CMovable::goUp() {
    if(hasProperty(EntityProperty::FLYING)) {
        body.velY -= accelerationY;
        
        if(body.velY < -_movementSpeeds[_movementState])
            body.velY = -_movementSpeeds[_movementState];
    } else
        jump();
    
    hasWalkedY = true;
}

void CMovable::goDown() {
    if(hasProperty(EntityProperty::FLYING)) {
        body.velY += accelerationY;
        
        if(body.velY > _movementSpeeds[_movementState])
                body.velY = _movementSpeeds[_movementState];
    }
    
    hasWalkedY = true;
}

void CMovable::toggleNoclip() {
    toggleProperty(EntityProperty::COLLIDABLE);
    toggleProperty(EntityProperty::FLYING);
    if (hasProperty(EntityProperty::COLLIDABLE))
        setTransparency(255);
    else
        setTransparency(128);
}

void CMovable::jump() {
    if (!collisionSides.bottom)
        return;
    
    body.velY -= accelerationY;
    if(body.velY < -jumpPower)
        body.velY = -jumpPower;
    
}

void CMovable::_doLogic() {
    CEntity::_doLogic();
    
    if(!hasWalkedX) {
        if(body.velX < 0) {
            body.velX += stoppingAccelerationX;
            if(body.velX >= 0)
                body.velX = 0.0f;
        } else {
            body.velX -= stoppingAccelerationX;
            if(body.velX <= 0)
                body.velX = 0.0f;
        }
    }
    
    if(!hasWalkedY) {
        if(hasProperty(EntityProperty::FLYING)) {
            if(body.velY < 0) {
                body.velY += stoppingAccelerationY;
                if(body.velY >= 0)
                    body.velY = 0.0f;
            } else {
                body.velY -= stoppingAccelerationY;
                if(body.velY <= 0)
                    body.velY = 0.0f;
            }
        }
    }
    
    hasWalkedX = false;
    hasWalkedY = false;
}

bool CMovable::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CEntity::_collisionLogic(target, collisionSides);
    bool collision = true;
    
    return parentCollision && collision;
}