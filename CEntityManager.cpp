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

CEntityManager::CEntityManager() : entityID(0), renderFlags(0) {
}

CEntity* CEntityManager::addEntity(SDL_Rect rect, SDL_Color color) {
    EntityVector[std::to_string(++entityID)] = new CEntity(rect, color);
    return EntityVector[std::to_string(entityID)];
}

CEntity* CEntityManager::addEntity(SDL_Rect rect, std::string spriteKey, CAssetManager* assetManager) {
    EntityVector[std::to_string(++entityID)] = new CEntity(rect, spriteKey, assetManager);
    return EntityVector[std::to_string(entityID)];
}

void CEntityManager::addEntity(CEntity* entity) {
    EntityVector[std::to_string(++entityID)] = entity;
}

void CEntityManager::addParticle(SDL_Rect rect, SDL_Color color, int livingTime) {
    ParticleVector.push_back(new CParticle(rect, color, livingTime));
}

void CEntityManager::addParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, int particleLivingTime, float velocity) {
    ParticleEmitterVector.push_back(new CParticleEmitter(rect, color, amount, frequency, livingTime, particleLivingTime, velocity));
}

void CEntityManager::addRenderFlag(int renderFlag) {
    renderFlags |= renderFlag;
}
void CEntityManager::removeRenderFlag(int renderFlag) {
    if(renderFlags & renderFlag) toggleRenderFlag(renderFlag);
}
void CEntityManager::toggleRenderFlag(int renderFlag) {
    renderFlags ^= renderFlag;
}

void CEntityManager::onRender(SDL_Renderer *renderer, CCamera* camera) {
    for (auto &i: ParticleVector)
        i->onRender(renderer, camera, renderFlags);
    
    for (auto &i: EntityVector)
        i.second->onRender(renderer, camera, renderFlags);
}

void CEntityManager::onLoop() {
    {
        auto i = EntityVector.begin();
        while(i != EntityVector.end()) {
            (*i).second->onLoop(&EntityVector);
            if((*i).second->toRemove) {
                delete (*i).second;
                EntityVector.erase(i->first);
                //EntityVector.erase(std::remove(EntityVector.begin(), EntityVector.end(), (*i)), EntityVector.end());
            }
            else
                ++i;
        }
    }
    
    {
        auto i = ParticleEmitterVector.begin();
        while(i != ParticleEmitterVector.end()) {
            (*i)->onLoop(this);
            if((*i)->toRemove)
                ParticleEmitterVector.erase(std::remove(ParticleEmitterVector.begin(), ParticleEmitterVector.end(), (*i)), ParticleEmitterVector.end());
            else
                ++i;
                
        }
    }
    
    {
        auto i = ParticleVector.begin();
        while(i != ParticleVector.end()) {
            (*i)->onLoop(&EntityVector);
            if((*i)->toRemove)
                ParticleVector.erase(std::remove(ParticleVector.begin(), ParticleVector.end(), (*i)), ParticleVector.end());
            else
                ++i;
        }
    }
}

void CEntityManager::onCleanup() {
    
    entityCleanup();
    particleEmitterCleanup();
    particleCleanup();
}

void CEntityManager::entityCleanup() {
    auto i = EntityVector.begin();
    while(i != EntityVector.end()) {
        delete i->second;
        EntityVector.erase(i++->first);
    }
    EntityVector.clear();
}

void CEntityManager::particleEmitterCleanup() {
    auto i = ParticleEmitterVector.begin();
    while(i != ParticleEmitterVector.end()) {
        
        delete *i;
        i = ParticleEmitterVector.erase(i);
        
    }
    ParticleEmitterVector.clear();
}

void CEntityManager::particleCleanup() {
    auto i = ParticleVector.begin();
    while(i != ParticleVector.end()) {
        
        delete *i;
        i = ParticleVector.erase(i);
        
    }
    ParticleVector.clear();
}