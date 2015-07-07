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
#include "CSpriteContainer.h"

CEntityManager::CEntityManager() : entityID(0), renderFlags(0) {
}

CEntity* CEntityManager::addEntity(Box rect, sf::Color color, std::string name /* = "" */) {
    if(name == "") {
        CEntity* tempEntity = new CEntity(rect, color);
        addEntity(tempEntity, name);
        return tempEntity;
    } else {
        CEntity* tempEntity = new CEntity(rect, color);
        addEntity(tempEntity, name);
        return tempEntity;
    }
}

CEntity* CEntityManager::addEntity(Box rect, std::string spriteKey, std::string name /* = "" */) {
    if(name == "") {
        CEntity* tempEntity = new CEntity(rect, spriteKey);
        addEntity(tempEntity, name);
        return tempEntity;
    } else {
        CEntity* tempEntity = new CEntity(rect, spriteKey);
        addEntity(tempEntity, name);
        return tempEntity;
    }
}

std::string CEntityManager::addEntity(CEntity* entity, std::string name /* = "" */) {
    if(name == "") {
        name = "5:" + std::to_string(++entityID);
        _EntityVector[name] = entity;
    } else if(_EntityVector.find(name) == _EntityVector.end())
        _EntityVector[name] = entity;
    else {  // Just overwrite the entity
        auto i = _EntityVector.find(name);
        delete i->second;
        _EntityVector[name] = entity;
    }
    
    entity->say(name, "TESTFONT", 0);
    return name;
}

CEntity* CEntityManager::getEntity(std::string name) {
    if(name == "") {
        std::cout << "Couldn't get entity because it had no name.\n";
        return nullptr;
    } else if(_EntityVector.find(name) == _EntityVector.end()) {
        std::cout << "Couldn't get entity: " << name << ", because it doesn't exist.";
        return nullptr;
    } else {
        return _EntityVector[name];
    }
}

CEntity* CEntityManager::getEntityAtCoordinate(int x, int y) {
    for (auto &i: _EntityVector) {
        if(i.second->coordinateCollision(x, y, 1, 1))
           return i.second;
    }
    return nullptr;
}

std::string CEntityManager::getNameOfEntity(CEntity *entity) {
    for (auto &i: _EntityVector) {                              // There must be a better way of doing this
        if(i.second == entity)
            return i.first;
    }
}

void CEntityManager::addParticle(Box rect, sf::Color color, int livingTime) {
    _ParticleVector.push_back(new CParticle(rect, color, livingTime));
}

void CEntityManager::addParticle(CParticle *particle) {
    _ParticleVector.push_back(particle);
}

void CEntityManager::addParticleEmitter(sf::IntRect rect, sf::Color color, int type, int amount, int frequency, int livingTime, int particleLivingTime, ParticleVelocity velocity) {
    _ParticleEmitterVector.push_back(new CParticleEmitter(rect, color, type, amount, frequency, livingTime, particleLivingTime, velocity));
}

void CEntityManager::addGuiText(CGuiText* guiText) {
    _GuiTextVector.push_back(guiText);
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

void CEntityManager::onRender(CWindow* window, CCamera* camera) {
    for (auto &i: _ParticleVector)
        i->onRender(window, camera, renderFlags);
    
    for (auto &i: _EntityVector)
        i.second->onRender(window, camera, renderFlags);
    
    for (auto &i: _EntityVector)
        i.second->renderAdditional(window, camera, renderFlags);
    
    for (auto &i: _GuiTextVector)
        i->onRender(window, camera);
}

// Temp

void CEntityManager::splitEntityToParticles(CEntity* target) {
    auto offset = target->getSpriteContainer()->getSprite()->getOffset();
    CSprite* tempSprite1 = new CSprite(target->getSpriteContainer()->getSprite()->getSpriteSheet(),
                                       Box{
                                           offset->x,
                                           offset->y,
                                           offset->w / 2,
                                           offset->h / 2});
    CSprite* tempSprite2 = new CSprite(target->getSpriteContainer()->getSprite()->getSpriteSheet(),
                                       Box{
                                           offset->x + offset->w / 2,
                                           offset->y,
                                           offset->w / 2,
                                           offset->h / 2});
    CSprite* tempSprite3 = new CSprite(target->getSpriteContainer()->getSprite()->getSpriteSheet(),
                                       Box{
                                           offset->x,
                                           offset->y + offset->h / 2,
                                           offset->w / 2,
                                           offset->h / 2});
    CSprite* tempSprite4 = new CSprite(target->getSpriteContainer()->getSprite()->getSpriteSheet(),
                                       Box{
                                           offset->x + offset->w / 2,
                                           offset->y + offset->h / 2,
                                           offset->w / 2,
                                           offset->h / 2});
    std::string sprite1 = CAssetManager::addSprite(tempSprite1);
    std::string sprite2 = CAssetManager::addSprite(tempSprite2);
    std::string sprite3 = CAssetManager::addSprite(tempSprite3);
    std::string sprite4 = CAssetManager::addSprite(tempSprite4);
    
    CSpriteContainer* tempSpriteContainer1 = new CSpriteContainer(sprite1);
    CSpriteContainer* tempSpriteContainer2 = new CSpriteContainer(sprite2);
    CSpriteContainer* tempSpriteContainer3 = new CSpriteContainer(sprite3);
    CSpriteContainer* tempSpriteContainer4 = new CSpriteContainer(sprite4);
    
    std::string spriteContainer1 = CAssetManager::addSpriteContainer(tempSpriteContainer1);
    std::string spriteContainer2 = CAssetManager::addSpriteContainer(tempSpriteContainer2);
    std::string spriteContainer3 = CAssetManager::addSpriteContainer(tempSpriteContainer3);
    std::string spriteContainer4 = CAssetManager::addSpriteContainer(tempSpriteContainer4);
    
    int explosionForce = 10;
    int tempForRand = 5;
    int livingTime = 2;
    
    CParticle* tempParticle1 = new CParticle(Box{
        target->body.getX(),
        target->body.getY(),
        target->body.getW() / 2,
        target->body.getH() / 2},
                                             spriteContainer1,
                                             livingTime + rand() % 3 - 1);
    tempParticle1->body.velX = rand() % explosionForce - tempForRand;
    tempParticle1->body.velY = rand() % explosionForce - tempForRand;
    if(target->hasProperty(EntityProperty::FLIP)) tempParticle1->addProperty(EntityProperty::FLIP);
    CParticle* tempParticle2 = new CParticle(Box{
        target->body.getX() + target->body.getW() / 2,
        target->body.getY(),
        target->body.getW() / 2,
        target->body.getH() / 2},
                                             spriteContainer2,
                                             livingTime + rand() % 3 - 1);
    tempParticle2->body.velX = rand() % explosionForce - tempForRand;
    tempParticle2->body.velY = rand() % explosionForce - tempForRand;
    if(target->hasProperty(EntityProperty::FLIP)) tempParticle2->addProperty(EntityProperty::FLIP);
    CParticle* tempParticle3 = new CParticle(Box{
        target->body.getX(),
        target->body.getY() + target->body.getH() / 2,
        target->body.getW() / 2,
        target->body.getH() / 2},
                                             spriteContainer3,
                                             livingTime + rand() % 3 - 1);
    tempParticle3->body.velX = rand() % explosionForce - tempForRand;
    tempParticle3->body.velY = rand() % explosionForce - tempForRand;
    if(target->hasProperty(EntityProperty::FLIP)) tempParticle3->addProperty(EntityProperty::FLIP);
    CParticle* tempParticle4 = new CParticle(Box{
        target->body.getX() + target->body.getW() / 2,
        target->body.getY() + target->body.getH() / 2,
        target->body.getW() / 2,
        target->body.getH() / 2},
                                             spriteContainer4,
                                             livingTime + rand() % 3);
    tempParticle4->body.velX = rand() % explosionForce - tempForRand;
    tempParticle4->body.velY = rand() % explosionForce - tempForRand;
    if(target->hasProperty(EntityProperty::FLIP)) tempParticle4->addProperty(EntityProperty::FLIP);
    
    addParticle(tempParticle1);
    addParticle(tempParticle2);
    addParticle(tempParticle3);
    addParticle(tempParticle4);
}

void CEntityManager::onLoop() {
    {
        auto i = _EntityVector.begin();
        while(i != _EntityVector.end()) {
            auto target = (*i).second;
            target->onLoop(&_EntityVector);
            
            if(target->isDead() && target->hasSprite()) {
                splitEntityToParticles(target);
                _DeadEntitiesVector[(*i).first] = (*i).second;
                _EntityVector.erase(i++);
            }
            
            
            if(target->toRemove) {
                delete target;
                _EntityVector.erase(i++);
            } else
                ++i;
        }
    }
    
    {
        auto i = _ParticleEmitterVector.begin();
        while(i != _ParticleEmitterVector.end()) {
            (*i)->onLoop(this);
            if((*i)->toRemove) {
                delete *i;
                _ParticleEmitterVector.erase(std::remove(_ParticleEmitterVector.begin(), _ParticleEmitterVector.end(), (*i)), _ParticleEmitterVector.end());
            } else
                ++i;
        }
    }
    
    {
        auto i = _ParticleVector.begin();
        while(i != _ParticleVector.end()) {
            (*i)->onLoop(&_EntityVector);
            if((*i)->toRemove) {
                delete *i;
                _ParticleVector.erase(std::remove(_ParticleVector.begin(), _ParticleVector.end(), (*i)), _ParticleVector.end());
            } else
                ++i;
        }
    }
    
    {
        auto i = _GuiTextVector.begin();
        while(i != _GuiTextVector.end()) {
            (*i)->onLoop();
            if((*i)->toRemove) {
                delete *i;
                _GuiTextVector.erase(std::remove(_GuiTextVector.begin(), _GuiTextVector.end(), (*i)), _GuiTextVector.end());
            } else
                ++i;
        }
    }
}

void CEntityManager::onCleanup() {
    
    entityID = 0;
    
    entityCleanup();
    particleEmitterCleanup();
    particleCleanup();
}

void CEntityManager::entityCleanup() {
    {
        auto i = _EntityVector.begin();
        while(i != _EntityVector.end()) {
            delete i->second;
            _EntityVector.erase(i++->first);
        }
        _EntityVector.clear();
    }
    
    {
        auto i = _DeadEntitiesVector.begin();
        while(i != _DeadEntitiesVector.end()) {
            delete i->second;
            _DeadEntitiesVector.erase(i++->first);
        }
        _DeadEntitiesVector.clear();
    }
}

void CEntityManager::particleEmitterCleanup() {
    auto i = _ParticleEmitterVector.begin();
    while(i != _ParticleEmitterVector.end()) {
        delete *i;
        i = _ParticleEmitterVector.erase(i);
    }
    _ParticleEmitterVector.clear();
}

void CEntityManager::particleCleanup() {
    auto i = _ParticleVector.begin();
    while(i != _ParticleVector.end()) {
        delete *i;
        i = _ParticleVector.erase(i);
    }
    _ParticleVector.clear();
}

void CEntityManager::guiTextCleanup() {
    auto i = _GuiTextVector.begin();
    while(i != _GuiTextVector.end()) {
        delete *i;
        i = _GuiTextVector.erase(i);
    }
    _GuiTextVector.clear();
}