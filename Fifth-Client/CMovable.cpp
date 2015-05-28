//
//  CMovable.cpp
//  Fifth-Client
//
//  Created by Lucas Karlsson on 27/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CMovable.h"
#include "Define.h"

CMovable::CMovable(sf::IntRect rect, sf::Color color) : CEntity(rect, color) {

}

CMovable::CMovable(sf::IntRect rect, std::string spriteKey) : CEntity(rect, spriteKey){

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

void CMovable::jump() {
    if (!collisionBottom)
        return;
    
    body.velY -= accelerationY;
    if(body.velY < -maxSpeed)
        body.velY = -maxSpeed;
    
}


void CMovable::checkMovement() {
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












