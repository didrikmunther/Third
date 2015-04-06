//
//  CEntityManager.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <iostream>
#include "CEntityManager.h"
#include "CCamera.h"

CEntityManager::CEntityManager() {
}

void CEntityManager::addEntity(SDL_Rect rect, SDL_Color color) {
    EntityVector.push_back(std::make_shared<CEntity>(rect, color));
}

void CEntityManager::addEntity(std::shared_ptr<CEntity> entity) {
    EntityVector.push_back(entity);
}

void CEntityManager::addParticle(SDL_Rect rect, SDL_Color color, int livingTime) {
    ParticleVector.push_back(std::make_shared<CParticle>(rect, color, livingTime));
}

void CEntityManager::addParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, float velocity) {
    ParticleEmitterVector.push_back(std::make_shared<CParticleEmitter>(rect, color, amount, frequency, livingTime, velocity));
}

void CEntityManager::onRender(SDL_Renderer *renderer, CCamera* camera) {
    for (auto &i: ParticleVector)
        i->onRender(renderer, camera);
    
    for (auto &i: EntityVector)
        i->onRender(renderer, camera);
}

void CEntityManager::onLoop() {
    std::cout << "Entities: " << EntityVector.size() << ", Particles: " << ParticleVector.size()  << ", Emittors: " << ParticleEmitterVector.size() << std::endl;
    
    auto i = EntityVector.begin();
    while(i != EntityVector.end()) {
        (*i)->onLoop(&EntityVector);
        if((*i)->toRemove)
            EntityVector.erase(std::remove(EntityVector.begin(), EntityVector.end(), (*i)), EntityVector.end());
        else
            ++i;
    }
    
    auto i3 = ParticleEmitterVector.begin();
    while(i3 != ParticleEmitterVector.end()) {
        (*i3)->onLoop(this);
        if((*i3)->toRemove)
            ParticleEmitterVector.erase(std::remove(ParticleEmitterVector.begin(), ParticleEmitterVector.end(), (*i3)), ParticleEmitterVector.end());
        else
            ++i3;
            
    }
    
    auto i2 = ParticleVector.begin();
    while(i2 != ParticleVector.end()) {
        (*i2)->onLoop(&EntityVector);
        if((*i2)->toRemove)
            ParticleVector.erase(std::remove(ParticleVector.begin(), ParticleVector.end(), (*i2)), ParticleVector.end());
        else
            ++i2;
    }
}

void CEntityManager::onCleanup() {
    EntityVector.clear();
    ParticleVector.clear();
    ParticleEmitterVector.clear();
}

void CEntityManager::particleCleanup() {
    //ParticleVector.clear();
    ParticleEmitterVector.clear();
}