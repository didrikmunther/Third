//
//  CEnemy.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CEnemy.h"

CEnemy::CEnemy(sf::IntRect rect, sf::Color color) :
    CNpc(rect, color) {
        _initMovementValues();
}

CEnemy::CEnemy(sf::IntRect rect, std::string spriteKey) :
    CNpc(rect, spriteKey) {
        _initMovementValues();
}

void CEnemy::_initMovementValues() {
    maxSpeed = 5.0f;
    jumpPower = 15.0f;
    accelerationX = 1.5f;
    accelerationY = 100.0f;
    stoppingAccelerationX = accelerationX * 2;
    sneakSpeed = (float)maxSpeed / 2.0f;
}

void CEnemy::_doLogic() {
    
    isSneaking = true;
    
    if(target != nullptr) {
        if(body.getX() > target->body.getX())
            goLeft();
        else if(body.getX() < target->body.getX())
            goRight();
        
        if(body.getY() > target->body.getY())
            jump();
    }
    
    checkMovement();
}

void CEnemy::setTarget(CEntity *target) {
    this->target = target;
}

CEntity* CEnemy::getTarget() {
    return target;
}