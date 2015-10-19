//
//  CEntityManager.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <sstream>

#include "CEntityManager.h"
#include "CCamera.h"
#include "CSpriteContainer.h"
#include "NFile.h"
#include "NSurface.h"
#include "CEntity.h"
#include "CSprite.h"
#include "CGuiText.h"
#include "CBackground.h"
#include "CInstance.h"


CEntityManager::CEntityManager() : entityID(0), _gridSize(32) {
    renderFlags = RenderFlags::CLEAR |
                  RenderFlags::RENDER_COMBAT_TEXT;
}

std::string CEntityManager::addEntity(CEntity* entity, std::string name /* = "" */) {
    if(name == "") {
        name = "5:" + std::to_string(++entityID);
        _entities[name] = entity;
    } else if(_entities.find(name) == _entities.end())
        _entities[name] = entity;
    else {  // Just overwrite the entity
        auto i = _entities.find(name);
        delete i->second;
        _entities[name] = entity;
    }
    
    entity->entityManager = this;
    
    //entity->say(name, "TESTFONT", ChatBubbleType::SAY);
    return name;
}

CEntity* CEntityManager::getEntity(std::string name) {
    if(_entities.find(name) == _entities.end()) {
        NFile::log(LogType::WARNING, "Couldn't get entity: ", name, ", because it doesn't exist.");
        return nullptr;
    } else {
        return _entities[name];
    }
}

CEntity* CEntityManager::createEntity(Box box, Color color) {
    return new CEntity(box, color);
}

CEntity* CEntityManager::createEntity(Box box, std::string spriteKey) {
    return new CEntity(box, spriteKey);
}

CEntity* CEntityManager::getEntityAtCoordinate(int x, int y) {
    for (auto &i: _entities) {
        if(i.second->coordinateCollision(x, y, 1, 1))
           return i.second;
    }
    return nullptr;
}

std::string CEntityManager::getNameOfEntity(CEntity *entity) {
    for (auto &i: _entities) {                              // There must be a better way of doing this
        if(i.second == entity)
            return i.first;
    }
    return "";
}

void CEntityManager::addParticle(CEntity *particle) {
    _particles.push_back(particle);
}

void CEntityManager::addGuiText(CGuiText* guiText) {
    _guiTextElements.push_back(guiText);
}

void CEntityManager::addBackground(std::string name, CBackground* background) {
    _backgrounds[name] = background;
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
    
    for(auto &i: _backgrounds) {
        i.second->onRender(window, camera);
    }
    
    if(renderFlags & RenderFlags::COLLISION_GRID) {
        for(int y = - 2; y < (window->getHeight() - (window->getHeight() % _gridSize)) / _gridSize + 2; y++) {
            for(int x = - 2; x < (window->getWidth() - (window->getWidth() % _gridSize)) / _gridSize + 2; x++) {
                NSurface::renderRect(x * _gridSize - camera->offsetX() % _gridSize, (y * _gridSize) - (camera->offsetY() % _gridSize), 1, _gridSize, window, 100, 200, 0);
                NSurface::renderRect(x * _gridSize - camera->offsetX() % _gridSize, (y * _gridSize) - (camera->offsetY() % _gridSize), _gridSize, 1, window, 100, 200, 0);
            }
        }
    }
    
    for (auto &i: _particles)
        i->onRender(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _entities)
        i.second->onRender(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _entities)
        if(!i.second->toRemove)
            i.second->renderAdditional(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _particles)
        if(!i->toRemove)
            i->renderAdditional(window, camera, (RenderFlags)renderFlags);
    
    for (auto &i: _deadEntities)
        if(!i.second->toRemove)
            i.second->renderAdditional(window, camera, (RenderFlags)renderFlags);
    
    if(renderFlags & RenderFlags::COLLISION_AREA) {
        for(auto &i: _entities) {
            auto target = i.second;
            int targetX = target->body->getX() + target->body->velX;
            int targetY = target->body->getY() + target->body->velY;
            int targetW = target->body->getW() + target->body->velX;
            int targetH = target->body->getH() + target->body->velY;
            NSurface::renderRect(targetX - camera->offsetX(), targetY - camera->offsetY(), targetW, targetH, window, 100, 200, 100, 100);
        }
        
        for(auto &i: _particles) {
            auto target = i;
            int targetX = target->body->getX() + target->body->velX;
            int targetY = target->body->getY() + target->body->velY;
            int targetW = target->body->getW() + target->body->velX;
            int targetH = target->body->getH() + target->body->velY;
            NSurface::renderRect(targetX - camera->offsetX(), targetY - camera->offsetY(), targetW, targetH, window, 100, 200, 100, 100);
        }
    }
    
    if(renderFlags & RenderFlags::COLLISION_FULL_AREA) {
        
    }
    
    if(renderFlags & RenderFlags::ENTITY_GRID) {
        for(auto &i: _entities) {
            for(auto &grid: i.second->gridCoordinates) {
                NSurface::renderRect(grid.x * _gridSize - camera->offsetX(), grid.y * _gridSize - camera->offsetY(), _gridSize, _gridSize, window, 0, 100, 100, 100);
            }
        }
        
        for(auto &i: _particles) {
            for(auto &grid: i->gridCoordinates) {
                NSurface::renderRect(grid.x * _gridSize - camera->offsetX(), grid.y * _gridSize - camera->offsetY(), _gridSize, _gridSize, window, 0, 100, 100, 100);
            }
        }
    }
    
    for (auto &i: _guiTextElements)
        i->onRender(window, camera, (RenderFlags)renderFlags);
}

// Temp
void CEntityManager::splitEntityToParticles(CEntity* target) {
    auto offset = target->getSpriteContainer()->getSprite()->getSource();
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
    
//    int explosionForce = 10;
//    int tempForRand = 5;
//    int livingTime = 2;
    
//    CEntity* tempParticle1 = new CEntity(Box{
//        target->body->getX(),
//        target->body->getY(),
//        target->body->getW() / 2,
//        target->body->getH() / 2},
//        spriteContainer1);
//    tempParticle1->addComponent<EParticle>(livingTime + rand() % 3 - 1);
//    tempParticle1->body->velX = rand() % explosionForce - tempForRand;
//    tempParticle1->body->velY = rand() % explosionForce - tempForRand;
//    if(target->hasProperty(EntityProperty::FLIP)) tempParticle1->addProperty(EntityProperty::FLIP);
//    CEntity* tempParticle2 = new CEntity(Box{
//        target->body->getX() + target->body->getW() / 2,
//        target->body->getY(),
//        target->body->getW() / 2,
//        target->body->getH() / 2},
//        spriteContainer2);
//    tempParticle2->addComponent<EParticle>(livingTime + rand() % 3 - 1);
//    tempParticle2->body->velX = rand() % explosionForce - tempForRand;
//    tempParticle2->body->velY = rand() % explosionForce - tempForRand;
//    if(target->hasProperty(EntityProperty::FLIP)) tempParticle2->addProperty(EntityProperty::FLIP);
//    CEntity* tempParticle3 = new CEntity(Box{
//        target->body->getX(),
//        target->body->getY() + target->body->getH() / 2,
//        target->body->getW() / 2,
//        target->body->getH() / 2},
//        spriteContainer3);
//    tempParticle3->addComponent<EParticle>(livingTime + rand() % 3 - 1);
//    tempParticle3->body->velX = rand() % explosionForce - tempForRand;
//    tempParticle3->body->velY = rand() % explosionForce - tempForRand;
//    if(target->hasProperty(EntityProperty::FLIP)) tempParticle3->addProperty(EntityProperty::FLIP);
//    CEntity* tempParticle4 = new CEntity(Box{
//        target->body->getX() + target->body->getW() / 2,
//        target->body->getY() + target->body->getH() / 2,
//        target->body->getW() / 2,
//        target->body->getH() / 2},
//        spriteContainer4);
//    tempParticle4->addComponent<EParticle>(livingTime + rand() % 3 - 1);
//    tempParticle4->body->velX = rand() % explosionForce - tempForRand;
//    tempParticle4->body->velY = rand() % explosionForce - tempForRand;
//    if(target->hasProperty(EntityProperty::FLIP)) tempParticle4->addProperty(EntityProperty::FLIP);
//    
//    addParticle(tempParticle1);
//    addParticle(tempParticle2);
//    addParticle(tempParticle3);
//    addParticle(tempParticle4);
}

void loopX(int x1, int x2, int y, std::vector<GridCoordinates>* toManipulate) {
    for(int x = x1; x < x2; x++) {
        toManipulate->push_back(GridCoordinates{x, y});
    }
}

int getBiggest(std::vector<int> intVector) {
    if(intVector.size() <= 0)
        return 0;
    int biggest = intVector[0];
    for(auto &i: intVector) {
        if(i > biggest)
            biggest = i;
    }
    return biggest;
}

int getSmallest(std::vector<int> intVector) {
    if(intVector.size() <= 0)
        return 0;
    int smallest = intVector[0];
    for(auto &i: intVector) {
        if(i < smallest)
            smallest = i;
    }
    return smallest;
}

std::vector<GridCoordinates> getGrid(CEntity* target, int gridSize) {
    
    int x = target->body->getX();
    int y = target->body->getY();
    int w = target->body->getW();
    int h = target->body->getH();
    
    int x2 = target->body->getX() + target->body->velX;
    int y2 = target->body->getY() + target->body->velY;
    
    int targetX = getSmallest({x, x2});
    int targetY = getSmallest({y, y2});
    int targetW = getBiggest({x, x2}) - getSmallest({x, x2}) + w;
    int targetH = getBiggest({y, y2}) - getSmallest({y, y2}) + h;
    
    int upperLeftCorner[2]  = {(targetX - (targetX % gridSize)) / gridSize,
        (targetY - (targetY % gridSize)) / gridSize};
    //int upperRightCorner[2] = {(targetX + targetW - ((targetX + targetW) % gridSize)) / gridSize,
    //    (targetY - (targetY % gridSize)) / gridSize};
    int lowerLeftCorner[2]  = {(targetX - (targetX % gridSize)) / gridSize,
        (targetY + targetH - ((targetY + targetH) % gridSize)) / gridSize};
    int lowerRightCorner[2] = {(targetX + targetW - ((targetX + targetW) % gridSize)) / gridSize,
        (targetY + targetH - ((targetY + targetH) % gridSize)) / gridSize};
    
    std::vector<GridCoordinates> toReturn;
    
    int margin = 1;
    
    if(upperLeftCorner[1] - margin < lowerLeftCorner[1] + margin) {
        for(int y = upperLeftCorner[1] - margin; y < lowerRightCorner[1] + margin; y++) {
            if(upperLeftCorner[0] - margin < lowerRightCorner[0] + margin) {
                loopX(upperLeftCorner[0] - margin, lowerRightCorner[0] + margin, y, &toReturn);
            } else {
                loopX(lowerRightCorner[0] - margin, upperLeftCorner[0] + margin, y, &toReturn);
            }
        }
    } else {
        for(int y = lowerRightCorner[1] - margin; y < upperLeftCorner[1] + margin; y++) {
            if(upperLeftCorner[0] - margin < lowerRightCorner[0] + margin) {
                loopX(upperLeftCorner[0] - margin, lowerRightCorner[0] + margin, y, &toReturn);
            } else {
                loopX(lowerRightCorner[0] - margin, upperLeftCorner[0] + margin, y, &toReturn);
            }
        }
    }
    
    return toReturn;
}

void CEntityManager::onLoop(CInstance* instance) {
    
    std::map <int, std::map <int, std::vector<CEntity*>>> _CollisionVector;
    
    {
        for(auto &particle: _particles) {
            particle->gridCoordinates.clear();
            
            particle->onLoop(instance);
            
            for(auto &coords: getGrid(particle, _gridSize)) {
                particle->gridCoordinates.push_back(GridCoordinates{coords.x, coords.y});
            }
        }
        
        for(auto &entity: _entities) {
            auto target = entity.second;
            target->gridCoordinates.clear();
            
            target->onLoop(instance);
            
            for(auto &coords: getGrid(target, _gridSize)) {
                _CollisionVector[coords.y][coords.x].push_back(target);
                target->gridCoordinates.push_back(GridCoordinates{coords.x, coords.y});
            }
        }
        
        for(auto &entity: _deadEntities) {
            auto target = entity.second;
            target->gridCoordinates.clear();
            
            target->onLoop(instance);
            
            for(auto &coords: getGrid(target, _gridSize)) {
                target->gridCoordinates.push_back(GridCoordinates{coords.x, coords.y});
            }
        }
    }
    
    {
        auto i = _entities.begin();
        while(i != _entities.end()) {
            auto target = (*i).second;
            
            std::vector<CEntity*> collisionMap;
            for (auto &coord: target->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                }
            }
            
            target->move(&collisionMap);
            
            if(target->isDead) {
                _deadEntities[(*i).first] = (*i).second;
                _entities.erase(i++);
                if(target->hasSprite())
                    splitEntityToParticles(target);
            } else if(target->toRemove) {
                delete target;
                _entities.erase(i++->first);
            } else {
                ++i;
            }
        }
    }
    
    {
        auto i = _deadEntities.begin();
        while(i != _deadEntities.end()) {
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
            
            target->move(&collisionMap);
            
            if(!target->isDead) {
                _entities[(*i).first] = (*i).second;
                _deadEntities.erase(i++);
            } else if((*i).second->toRemove) {
                delete (*i).second;
                _deadEntities.erase(i++);
            } else
                ++i;
        }
    }
    
    {
        auto i = _particles.begin();
        while(i != _particles.end()) {
            auto target = (*i);
            
            std::stringstream toSay;
            
            std::vector<CEntity*> collisionMap;
            for (auto &coord: target->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                    toSay << getNameOfEntity(entity) << ", ";
                }
            }
            
            target->move(&collisionMap);
            
            if(target->toRemove) {
                delete *i;
                _particles.erase(std::remove(_particles.begin(), _particles.end(), target), _particles.end());
            } else
                ++i;
        }
    }
    
    {
        auto i = _guiTextElements.begin();
        while(i != _guiTextElements.end()) {
            (*i)->onLoop();
            if((*i)->toRemove()) {
                delete *i;
                _guiTextElements.erase(std::remove(_guiTextElements.begin(), _guiTextElements.end(), (*i)), _guiTextElements.end());
            } else
                ++i;
        }
    }
    
    for(auto &i: _entities) { // Add delayed particles of all entities, like bullets
        for(auto &particle: i.second->particlesToAdd) {
            addParticle(particle);
        }
        i.second->particlesToAdd.clear();
    }
}

void CEntityManager::onCleanup() {
    
    NFile::log(LogType::ALERT, "Unloading entities!");
    
    entityID = 0;
    
    entityCleanup();
    particleCleanup();
}

void CEntityManager::entityCleanup() {
    {
        auto i = _entities.begin();
        while(i != _entities.end()) {
            delete i->second;
            _entities.erase(i++->first);
        }
        _entities.clear();
    }
    
    {
        auto i = _deadEntities.begin();
        while(i != _deadEntities.end()) {
            delete i->second;
            _deadEntities.erase(i++->first);
        }
        _deadEntities.clear();
    }
    
    {
        auto i = _backgrounds.begin();
        while(i != _backgrounds.end()) {
            delete i->second;
            _backgrounds.erase(i++->first);
        }
        _backgrounds.clear();
    }
}

void CEntityManager::particleCleanup() {
    auto i = _particles.begin();
    while(i != _particles.end()) {
        delete *i;
        i = _particles.erase(i);
    }
    _particles.clear();
}

void CEntityManager::guiTextCleanup() {
    auto i = _guiTextElements.begin();
    while(i != _guiTextElements.end()) {
        delete *i;
        i = _guiTextElements.erase(i);
    }
    _guiTextElements.clear();
}