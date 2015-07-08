//
//  CMovable.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CMovable.h"
#include "Define.h"

CMovable::CMovable(Box rect, sf::Color color) :
    CEntity(rect, color) {
    _initMovementValues();
}

CMovable::CMovable(Box rect, std::string spriteKey) :
    CEntity(rect, spriteKey) {
    _initMovementValues();
}

void CMovable::_initMovementValues() {
    maxSpeed = 10.0f;                   // Default values
    jumpPower = 10.0f;
    accelerationX = 1.5f;
    accelerationY = 100.0f;
    stoppingAccelerationX = accelerationX / 2;
    sneakSpeed = (float)maxSpeed / 2.0f;
    isSneaking = false;
    hasWalkedX = false;
    hasWalkedY = false;
}

void CMovable::goRight() {
    body.velX += accelerationX;
    if(isSneaking) {
        if(body.velX > sneakSpeed)
            body.velX = sneakSpeed;
    } else {
        if(body.velX > maxSpeed)
            body.velX = maxSpeed;
    }
    
    hasWalkedX = true;
}

void CMovable::goLeft() {
    body.velX -= accelerationX;
    if(isSneaking) {
        if(body.velX < -sneakSpeed)
            body.velX = -sneakSpeed;
    } else {
        if(body.velX < -maxSpeed)
            body.velX = -maxSpeed;
    }
    hasWalkedX = true;
}

void CMovable::goUp() {
    if(hasProperty(EntityProperty::FLYING)) {
        body.velY -= accelerationY;
        if(isSneaking) {
            if(body.velY < -sneakSpeed)
                body.velY = -sneakSpeed;
        } else {
            if(body.velY < -maxSpeed)
                body.velY = -maxSpeed;
        }
    } else {
        jump();
    }
    
    hasWalkedY = true;
}

void CMovable::goDown() {
    if(hasProperty(EntityProperty::FLYING)) {
        body.velY += accelerationY;
        
        if(isSneaking) {
            if(body.velY > sneakSpeed)
                body.velY = sneakSpeed;
        } else {
            if(body.velY > maxSpeed)
                body.velY = maxSpeed;
        }
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
    if (!collisionBottom)
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
                body.velY += accelerationY;
                if(body.velY >= 0)
                    body.velY = 0.0f;
            } else {
                body.velY -= accelerationY;
                if(body.velY <= 0)
                    body.velY = 0.0f;
            }
        }
    }
    
    hasWalkedX = false;
    hasWalkedY = false;
}

void CMovable::_collisionLogic(CEntity* target) {
    CEntity::_collisionLogic(target);
    
    
}