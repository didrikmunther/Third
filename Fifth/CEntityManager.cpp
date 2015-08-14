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
#include "NFile.h"
#include "NSurface.h"

CEntityManager::CEntityManager() : entityID(0), renderFlags(RenderFlags::CLEAR) {
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
    
    //entity->say(name, "TESTFONT", ChatBubbleType::SAY);
    return name;
}

CEntity* CEntityManager::getEntity(std::string name) {
    if(_EntityVector.find(name) == _EntityVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't get entity: ", name, ", because it doesn't exist.");
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

void CEntityManager::addRenderFlag(RenderFlags renderFlag) {
    renderFlags |= (int)renderFlag;
}
void CEntityManager::removeRenderFlag(RenderFlags renderFlag) {
    if(renderFlags & renderFlag) toggleRenderFlag(renderFlag);
}
void CEntityManager::toggleRenderFlag(RenderFlags renderFlag) {
    renderFlags ^= (int)renderFlag;
}

void CEntityManager::onRender(CWindow* window, CCamera* camera) {
    for (auto &i: _ParticleVector)
        i->onRender(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _EntityVector)
        i.second->onRender(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _EntityVector)
        if(!i.second->toRemove())
            i.second->renderAdditional(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _DeadEntitiesVector)
        if(!i.second->toRemove())
            i.second->renderAdditional(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _GuiTextVector)
        i->onRender(window, camera);
    
    int gridSize = 1024;
    
    for(int y = 0; y < (SCREEN_HEIGHT - (SCREEN_HEIGHT % gridSize)) / gridSize + 50; y++) {
        for(int x = 0; x < (SCREEN_WIDTH - (SCREEN_WIDTH % gridSize)) / gridSize + 50; x++) {
            NSurface::renderRect(x * gridSize - camera->offsetX(), y * gridSize - camera->offsetY(), 1, gridSize, *window->getRenderTexture(), 100, 200, 0);
            NSurface::renderRect(x * gridSize - camera->offsetX(), y * gridSize - camera->offsetY(), gridSize, 1, *window->getRenderTexture(), 100, 200, 0);
        }
    }
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

std::vector<GridCoordinates> getGrid(CEntity* target, int gridSize) {
    int upperLeftCorner[2]  = {(target->body.getX() - (target->body.getX() % gridSize)) / gridSize,
        (target->body.getY() - (target->body.getY() % gridSize)) / gridSize};
    int upperRightCorner[2] = {(target->body.getX() + target->body.getW() - ((target->body.getX() + target->body.getW()) % gridSize)) / gridSize,
        (target->body.getY() - (target->body.getY() % gridSize)) / gridSize};
    int lowerLeftCorner[2]  = {(target->body.getX() - (target->body.getX() % gridSize)) / gridSize,
        (target->body.getY() + target->body.getH() - ((target->body.getY() + target->body.getH()) % gridSize)) / gridSize};
    int lowerRightCorner[2] = {(target->body.getX() + target->body.getW() - ((target->body.getX() + target->body.getW()) % gridSize)) / gridSize,
        (target->body.getY() + target->body.getH() - ((target->body.getY() + target->body.getH()) % gridSize)) / gridSize};
    
    std::vector<GridCoordinates> toReturn;
    for(int y = upperLeftCorner[1] - 1; y < lowerRightCorner[1] + 1; y++) {
        for(int x = upperLeftCorner[0] - 1; x < lowerRightCorner[0] + 1; x++) {
            toReturn.push_back(GridCoordinates{x, y});
        }
    }
    
    return toReturn;
}

void CEntityManager::onLoop() {
    
    {
        _CollisionVector.clear();
        
        int gridSize = 16;
        
        for(auto &entity: _EntityVector) {
            auto target = entity.second;
            target->gridCoordinates.clear();
            
            for(auto &coords: getGrid(target, gridSize)) {
                _CollisionVector[coords.y][coords.x].push_back(target);
                target->gridCoordinates.push_back(GridCoordinates{coords.x, coords.y});
            }
        }
        
        for(auto &particle: _ParticleVector) {
            particle->gridCoordinates.clear();
            
            for(auto &coords: getGrid(particle, gridSize)) {
                particle->gridCoordinates.push_back(GridCoordinates{coords.x, coords.y});
            }
        }
        
        for(auto &entity: _DeadEntitiesVector) {
            auto target = entity.second;
            target->gridCoordinates.clear();
            
            for(auto &coords: getGrid(target, gridSize)) {
                target->gridCoordinates.push_back(GridCoordinates{coords.x, coords.y});
            }
        }
        
    }
    
    {
        auto i = _EntityVector.begin();
        while(i != _EntityVector.end()) {
            auto target = (*i).second;
            
            std::vector<CEntity*> collisionMap;
            std::vector<CEntity*> alreadyAdded;
            for (auto &coord: target->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                }
            }
            
            target->onLoop(&collisionMap);
            
            if(target->isDead() && target->hasSprite()) {
                splitEntityToParticles(target);
                _DeadEntitiesVector[(*i).first] = (*i).second;
                _EntityVector.erase(i++);
            }
            
            
            if(target->toRemove()) {
                delete target;
                _EntityVector.erase(i++);
            } else
                ++i;
        }
    }
    
    {
        auto i = _DeadEntitiesVector.begin();
        while(i != _DeadEntitiesVector.end()) {
            auto target = i->second;
            
            std::vector<CEntity*> collisionMap;
            std::vector<CEntity*> alreadyAdded;
            for (auto &coord: target->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                }
            }
            
            (*i).second->onLoop(&collisionMap);
            
            if((*i).second->toRemove()) {
                delete (*i).second;
                _DeadEntitiesVector.erase(i++);
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
            
            std::vector<CEntity*> collisionMap;
            for (auto &coord: (*i)->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                }
            }
            
            (*i)->onLoop(&collisionMap);
            if((*i)->toRemove()) {
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
            if((*i)->toRemove()) {
                delete *i;
                _GuiTextVector.erase(std::remove(_GuiTextVector.begin(), _GuiTextVector.end(), (*i)), _GuiTextVector.end());
            } else
                ++i;
        }
    }
}

void CEntityManager::onCleanup() {
    
    NFile::log(LogType::ALERT, "Unloading entities!");
    
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