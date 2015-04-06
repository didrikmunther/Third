//
//  CParticleEmitter.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CParticleEmitter.h"
#include "CEntityManager.h"
#include <iostream>

CParticleEmitter::CParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, float velocity) :
    rect(rect), color(color), amount(amount), frequency(frequency),
    livingTime(livingTime), velocity(velocity), creationTime(SDL_GetTicks()),
    toRemove(false), timer(SDL_GetTicks() - (frequency * 1000)) {
}

void CParticleEmitter::onLoop(CEntityManager *entityManager) {
    if(SDL_GetTicks() - creationTime > livingTime * 1000)
        toRemove = true;
    
    if(SDL_GetTicks() - timer > frequency * 1000) {
        timer += 1000;
        for (int i = 0; i < amount; i++) {
            entityManager->addParticle(rect, color, livingTime);
        }
    }
        
}