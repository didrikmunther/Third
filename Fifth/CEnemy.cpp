//
//  CEnemy.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CEnemy.h"
#include <iostream>

CEnemy::CEnemy(Box rect, SDL_Color color) :
    CNpc(rect, color) {
        _init();
}

CEnemy::CEnemy(Box rect, std::string spriteKey) :
    CNpc(rect, spriteKey) {
        _init();
}

void CEnemy::_init() {
    target = nullptr;
    setMovementState(MovementState::SNEAKING_MOVEMENT);
    jumpPower = 15.0f;
    accelerationX = 0.1f;
}

void CEnemy::renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    CNpc::renderAdditional(window, camera, renderFlags);
    
}

void CEnemy::setTarget(CEntity *target) {
    this->target = target;
}

CEntity* CEnemy::getTarget() {
    return target;
}

void CEnemy::_doLogic() {
    CNpc::_doLogic();
    
    if(target != nullptr) {
        if(body.getX() > target->body.getX())
            goLeft();
        else if(body.getX() < target->body.getX())
            goRight();
        
        if(collisionSides.right || collisionSides.left)
            jump();
    }
}

bool CEnemy::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CNpc::_collisionLogic(target, collisionSides);
    bool collision = true;
    
    return parentCollision && collision;
}