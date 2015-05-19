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
    _rect(rect), _color(color), _amount(amount), _frequency(frequency),
    _livingTime(livingTime), _particleLivingTime(particleLivingTime),
    _velocity(velocity), _creationTime(_clock.getElapsedTime().asMilliseconds()),
    toRemove(false), _timer(_clock.getElapsedTime().asMilliseconds() - (frequency * 1000)) {
}

void CParticleEmitter::onLoop(CEntityManager *entityManager) {
    if(_clock.getElapsedTime().asMilliseconds() - _creationTime > _livingTime * 1000) {
        toRemove = true;
        return;
    }
    
    if(_clock.getElapsedTime().asMilliseconds() - _timer > _frequency * 1000) {
        _timer += 1000;
        for (int i = 0; i < _amount; i++) {
            entityManager->addParticle(_rect, _color, _particleLivingTime);
        }
    }
        
}