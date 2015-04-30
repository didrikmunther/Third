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

CParticleEmitter::CParticleEmitter(sf::IntRect rect, sf::Color color, int amount, int frequency, int livingTime, int particleLivingTime, float velocity) :
    rect(rect), color(color), amount(amount), frequency(frequency),
    livingTime(livingTime), particleLivingTime(particleLivingTime),
    velocity(velocity), creationTime(clock.getElapsedTime().asMilliseconds()),
    toRemove(false), timer(clock.getElapsedTime().asMilliseconds() - (frequency * 1000)) {
}

void CParticleEmitter::onLoop(CEntityManager *entityManager) {
    if(clock.getElapsedTime().asMilliseconds() - creationTime > livingTime * 1000) {
        toRemove = true;
        return;
    }
    
    if(clock.getElapsedTime().asMilliseconds() - timer > frequency * 1000) {
        timer += 1000;
        for (int i = 0; i < amount; i++) {
            entityManager->addParticle(rect, color, particleLivingTime);
        }
    }
        
}