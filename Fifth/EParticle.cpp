//
//  EParticle.cpp
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EParticle.h"

#include "CEntity.h"


EParticle::EParticle(CEntity* parent, int livingTime) : EComponent(parent) {
    parent->addProperty(EntityProperty::FLIP_FREEZED);
    parent->collisionLayer = CollisionLayers::LAYER1;
    _livingTime = livingTime;
    _creationTime = SDL_GetTicks();
}

void EParticle::onLoop(CInstance* instance) {
    if(_livingTime > 0 && SDL_GetTicks() - _creationTime > _livingTime * 1000)
        parent->toRemove = true;
}