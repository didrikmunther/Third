//
//  ENpc.cpp
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "ENpc.h"
#include "EMovable.h"
#include "ELiving.h"

ENpc::ENpc(CEntity* parent) : EComponent(parent), _target(nullptr) {
}

void ENpc::onLoop(CInstance* instance) {
    if(_target && !_target->isDead && !_target->toRemove) {
        if(_parent->body.getX() > _target->body.getX()) {
            auto movable = _parent->movable;//_parent->getComponent<EMovable>();
            if(movable) {
                movable->goLeft();
            } else {
                _parent->body.velX -= 1;
            }
        }
        else if(_parent->body.getX() < _target->body.getX()) {
            auto movable = _parent->movable;//_parent->getComponent<EMovable>();
            if(movable) {
                movable->goRight();
            } else {
                _parent->body.velX += 1;
            }
        }
        if(_parent->collisionRight || _parent->collisionLeft) {
            auto movable = _parent->movable;//_parent->getComponent<EMovable>();
            if(movable) {
                movable->jump();
            }
        }
    }
}

void ENpc::onRender(CWindow* window, CCamera* camera) {
    
}

void ENpc::renderAdditional(CWindow* window, CCamera* camera) {
    
}

bool ENpc::collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides) {
    return true;
}

void ENpc::setTarget(CEntity* target) {
    if(target)
        _target = target;
}

CEntity* ENpc::getTarget() {
    
}
