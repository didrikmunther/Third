////
////  CParticleEmitter.cpp
////  Third
////
////  Created by Didrik Munther on 19/03/15.
////  Copyright (c) 2015 Didrik Munther. All rights reserved.
////
//
//#include "CParticleEmitter.h"
//#include "CEntityManager.h"
//#include "CUtilityParticle.h"
//#include <iostream>
//
//CParticleEmitter::CParticleEmitter(sf::IntRect rect, SDL_Color color, int type, int amount, int frequency, int livingTime, int particleLivingTime, ParticleVelocity velocity) :
//_rect(rect), _color(color), type(type), _amount(amount), _frequency(frequency),
//_livingTime(livingTime), _particleLivingTime(particleLivingTime),
//_velocity(velocity), _creationTime(_clock.getElapsedTime().asMilliseconds()),
//toRemove(false), _timer(_clock.getElapsedTime().asMilliseconds() - (frequency * 1000)) {
//}
//
////CParticleEmitter::CParticleEmitter(CParticle* particleTemplate, int amount, int frequency, int livingTime, ParticleVelocity velocity) :
////_rect(nullptr), _color(nullptr)
////{
////    
////}
//
//// Nothing of this is working right now
//
//void CParticleEmitter::onLoop(CEntityManager *entityManager) {
////    if(_clock.getElapsedTime().asMilliseconds() - _creationTime > _livingTime * 1000) {
////        toRemove = true;
////        return;
////    }
////    
////    if(_clock.getElapsedTime().asMilliseconds() - _timer > _frequency * 1000) {
////        _timer += 1000;
////        for (int i = 0; i < _amount; i++) {
////            CParticle* particle;
////            switch(type) {
////                case ParticleTypes::UTILITY_PARTICLE:
////                    particle = new CUtilityParticle(_rect, _color, _particleLivingTime);
////                    break;
////                default:
////                    particle = new CParticle(_rect, _color, _particleLivingTime);
////            }
////            particle->body.velX += _velocity.x;
////            particle->body.velY += _velocity.y;
////            
////            entityManager->addParticle(particle);
////        }
////    }
//    
//}