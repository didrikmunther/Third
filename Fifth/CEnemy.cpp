//
//  CEnemy.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CEnemy.h"

CEnemy::CEnemy(Box rect, sf::Color color) :
    CNpc(rect, color) {
        _initMovementValues();
}

CEnemy::CEnemy(Box rect, std::string spriteKey) :
    CNpc(rect, spriteKey) {
        _initMovementValues();
}

void CEnemy::_initMovementValues() {
    maxSpeed = 5.0f;
    jumpPower = 15.0f;
    accelerationX = 0.1f;
    accelerationY = 100.0f;
    stoppingAccelerationX = accelerationX * 2;
    sneakSpeed = (float)maxSpeed / 2.0f;
}

void CEnemy::setTarget(CEntity *target) {
    this->target = target;
}

CEntity* CEnemy::getTarget() {
    return target;
}

void CEnemy::_doLogic() {
    CNpc::_doLogic();
    
    isSneaking = true;
    
    if(target != nullptr) {
        if(body.getX() > target->body.getX())
            goLeft();
        else if(body.getX() < target->body.getX())
            goRight();
        
        //if(body.getY() > target->body.getY())
        if(collisionRight || collisionLeft)
            jump();
        
//        if(target->body.getY() < body.getY())
//            goUp();
//        else if(target->body.getY() > body.getY())
//            goDown();
    }
}

void CEnemy::_collisionLogic(CEntity* target) {
    CNpc::_collisionLogic(target);
}