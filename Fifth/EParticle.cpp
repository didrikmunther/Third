//
//  EParticle.cpp
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EParticle.h"
#include <SDL2/SDL.h>

EParticle::EParticle(CEntity* parent, int livingTime) : EComponent(parent), _livingTime(livingTime), _creationTime(SDL_GetTicks()) {
    
}

void EParticle::onLoop(CInstance* instance) {
    if(_parent->collisionBottom) {
        _parent->body.velX /= 2;
        if(_parent->body.velX > 0)
            if(_parent->body.velX < 0.000001)
                _parent->body.velX = 0;
        if(_parent->body.velX < 0)
            if(_parent->body.velX > -0.000001)
                _parent->body.velX = 0;
    }
    
    if(_livingTime > 0 && SDL_GetTicks() - _creationTime > _livingTime * 1000)
        _parent->toRemove = true;
}

void EParticle::onRender(CWindow* window, CCamera* camera) {
    
}

void EParticle::renderAdditional(CWindow* window, CCamera* camera) {
    
}

bool EParticle::collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides) {
    return true;
}
