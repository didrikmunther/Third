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

CEntity* CEntityManager::addEntity(SDL_Rect rect, SDL_Color color, std::string name /* = "" */) {
    if(name == "") {
        std::string id = "5:" + std::to_string(++entityID);
        EntityVector[id] = new CEntity(rect, color);
        return EntityVector[id];
    }
    else {
        EntityVector[name] = new CEntity(rect, color);
        return EntityVector[name];
    }
}

CEntity* CEntityManager::addEntity(SDL_Rect rect, std::string spriteKey, CAssetManager* assetManager, std::string name /* = "" */) {
    if(name == "") {
        std::string id = "5:" + std::to_string(++entityID);
        EntityVector[id] = new CEntity(rect, spriteKey, assetManager);
        return EntityVector[id];
    }
    else {
        EntityVector[name] = new CEntity(rect, spriteKey, assetManager);
        return EntityVector[name];
    }
}

void CEntityManager::addEntity(CEntity* entity, std::string name /* = "" */) {
    if(name == "") {
        std::string id = "5:" + std::to_string(++entityID);
        EntityVector[id] = entity;
    }
    else
        EntityVector[name] = entity;
}

void CEntityManager::addParticle(SDL_Rect rect, SDL_Color color, int livingTime) {
    ParticleVector.push_back(new CParticle(rect, color, livingTime));
}

void CEntityManager::addParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, int particleLivingTime, float velocity) {
    ParticleEmitterVector.push_back(new CParticleEmitter(rect, color, amount, frequency, livingTime, particleLivingTime, velocity));
}

void CEntityManager::addGuiText(CGuiText* guiText) {
    GuiTextVector.push_back(guiText);
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
    
    for (auto &i: GuiTextVector)
        i->onRender(renderer, camera);
}

void CEntityManager::onLoop() {
    {
        auto i = EntityVector.begin();
        while(i != EntityVector.end()) {
            (*i).second->onLoop(&EntityVector);
            if((*i).second->toRemove) {
                delete (*i).second;
                EntityVector.erase(i->first);
            }
            else
                ++i;
        }
    }
    
    {
        auto i = ParticleEmitterVector.begin();
        while(i != ParticleEmitterVector.end()) {
            (*i)->onLoop(this);
            if((*i)->toRemove) {
                delete *i;
                ParticleEmitterVector.erase(std::remove(ParticleEmitterVector.begin(), ParticleEmitterVector.end(), (*i)), ParticleEmitterVector.end());
            } else
                ++i;
                
        }
    }
    
    {
        auto i = ParticleVector.begin();
        while(i != ParticleVector.end()) {
            (*i)->onLoop(&EntityVector);
            if((*i)->toRemove) {
                delete *i;
                ParticleVector.erase(std::remove(ParticleVector.begin(), ParticleVector.end(), (*i)), ParticleVector.end());
            } else
                ++i;
        }
    }
    
    {
        auto i = GuiTextVector.begin();
        while(i != GuiTextVector.end()) {
            (*i)->onLoop();
            if((*i)->toRemove) {
                delete *i;
                GuiTextVector.erase(std::remove(GuiTextVector.begin(), GuiTextVector.end(), (*i)), GuiTextVector.end());
            }
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

void CEntityManager::guiTextCleanup() {
    auto i = GuiTextVector.begin();
    while(i != GuiTextVector.end()) {
        delete *i;
        i = GuiTextVector.erase(i);
    }
    GuiTextVector.clear();
}