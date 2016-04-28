//
//  CEntityManager.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <sstream>

#include "CEntityManager.h"
#include "CAssetManager.h"
#include "CCamera.h"
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
    
    return name;
}

CEntity* CEntityManager::getEntity(std::string name) {
    if(_entities.find(name) == _entities.end()) {
        //NFile::log(LogType::WARNING, "Couldn't get entity: ", name, ", because it doesn't exist.");
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

int CEntityManager::pushEntities(lua_State* L) {
    lua_newtable(L);
    int top = lua_gettop(L);
    
    for(auto& i: _entities) {
        auto key = i.first;
        auto value = i.second;
        
        luabridge::Stack<std::string>::push(L, key);
        luabridge::Stack<CEntity*>::push(L, value);
        lua_settable(L, top);
    }
    
    return 1;
}

CEntity* CEntityManager::getEntityAtCoordinate(int x, int y, CEntity* except /* = nullptr */) {
    for (auto &i: _entities) {
        if(i.second->coordinateCollision(x, y, 1, 1)) {
            if(i.second != except)
                return i.second;
        }
    }
    return nullptr;
}

std::vector<CEntity*> CEntityManager::getEntitiesAtCoordinate(int x, int y) {
    std::vector<CEntity*> toReturn = {};
    
    for (auto &i: _entities) {
        if(i.second->coordinateCollision(x, y, 1, 1))
            toReturn.push_back(i.second);
    }
    
    return toReturn;
}

std::string CEntityManager::getNameOfEntity(CEntity *entity) {
    for (auto &i: _entities) {                              // There must be a better way of doing this
        if(i.second == entity)
            return i.first;
    }
    return "";
}

CBackground* CEntityManager::getBackground(std::string name) {
    if(_backgrounds.find(name) == _backgrounds.end())
        return nullptr;
    else
        return _backgrounds[name];
}

void CEntityManager::addParticle(CEntity *particle) {
    particle->entityManager = this;
    
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
            NSurface::renderRect(targetX - camera->offsetX(), targetY - camera->offsetY(), targetW, targetH, window, 100, 200, 100, 50);
        }
        
        for(auto &i: _particles) {
            auto target = i;
            int targetX = target->body->getX() + target->body->velX;
            int targetY = target->body->getY() + target->body->velY;
            int targetW = target->body->getW() + target->body->velX;
            int targetH = target->body->getH() + target->body->velY;
            NSurface::renderRect(targetX - camera->offsetX(), targetY - camera->offsetY(), targetW, targetH, window, 100, 200, 100, 50);
        }
    }
    
    if(renderFlags & RenderFlags::COLLISION_FULL_AREA) {
        
    }
    
    if(renderFlags & RenderFlags::ENTITY_GRID) {
        for(auto &i: _entities) {
            for(auto &grid: i.second->gridCoordinates) {
                NSurface::renderRect(grid.x * _gridSize - camera->offsetX(), grid.y * _gridSize - camera->offsetY(), _gridSize, _gridSize, window, 0, 100, 100, 20);
            }
        }
        
        for(auto &i: _particles) {
            for(auto &grid: i->gridCoordinates) {
                NSurface::renderRect(grid.x * _gridSize - camera->offsetX(), grid.y * _gridSize - camera->offsetY(), _gridSize, _gridSize, window, 0, 100, 100, 20);
            }
        }
    }
    
    for (auto &i: _guiTextElements)
        i->onRender(window, camera, (RenderFlags)renderFlags);
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
//    std::vector<CEntity*> _CollisionVector[2048][2048];
    
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
        std::vector<CEntity*> collisionMap;
        
        auto i = _entities.begin();
        while(i != _entities.end()) {
            auto target = (*i).second;
            
            collisionMap.clear();
            
            for (auto &coord: target->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                }
            }
            
            target->move(&collisionMap, instance);
            
            if(target->isDead) {
                auto name = (*i).first;
                if(_deadEntities[name])
                    delete _deadEntities[name];
                _deadEntities[name] = (*i).second;
                _entities.erase(i++);
            } else if(target->toRemove) {
                delete target;
                _entities.erase(i++->first);
            } else {
                ++i;
            }
        }
    }
    
    {
        std::vector<CEntity*> collisionMap;
        
        auto i = _deadEntities.begin();
        while(i != _deadEntities.end()) {
            auto target = i->second;
            
            collisionMap.clear();
            
            for (auto &coord: target->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                }
            }
            
            target->move(&collisionMap, instance);
            
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
        std::vector<CEntity*> collisionMap;
        
        auto i = _particles.begin();
        while(i != _particles.end()) {
            auto target = (*i);
            
            collisionMap.clear();
            
            for (auto &coord: target->gridCoordinates) {
                for(auto &entity: _CollisionVector[coord.y][coord.x]){
                    if(std::find(collisionMap.begin(), collisionMap.end(), entity) != collisionMap.end())
                        continue;
                    collisionMap.push_back(entity);
                }
            }
            
            target->move(&collisionMap, instance);
            
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
}

void CEntityManager::onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance) {
    rapidjson::Value entityValues(rapidjson::kObjectType);
    for(auto& i: _entities) {
        rapidjson::Value entity(rapidjson::kObjectType);
        i.second->onSerialize(&entity, alloc, instance);
        entityValues.AddMember(rapidjson::Value(i.first.c_str(), *alloc), entity, *alloc);
    }
    
    rapidjson::Value backgroundValues(rapidjson::kObjectType);
    for(auto& i: _backgrounds) {
        rapidjson::Value background(rapidjson::kObjectType);
        i.second->onSerialize(&background, alloc, instance);
        backgroundValues.AddMember(rapidjson::Value(i.first.c_str(), *alloc), background, *alloc);
    }
    
    value->AddMember("entities", entityValues, *alloc);
    value->AddMember("backgrounds", backgroundValues, *alloc);
    value->AddMember("entityID", rapidjson::Value(entityID), *alloc);
}

void CEntityManager::onDeserialize(rapidjson::Value* value, CInstance* instance) {
    
    if(value->HasMember("entityID"))
        entityID = (*value)["entityID"].GetInt();
    
    if(value->HasMember("backgrounds")) {
        const rapidjson::Value& backgroundValues = (*value)["backgrounds"];
        
        for(rapidjson::Value::ConstMemberIterator i = backgroundValues.MemberBegin(); i != backgroundValues.MemberEnd(); i++) {
            const rapidjson::Value* backgroundValue = &i->value;
            
            auto background = getBackground(i->name.GetString());
            if(background == nullptr) {
                std::string spriteKey = "";
                assignString(backgroundValue, "spriteKey", &spriteKey);
                
                float parallax = 1;
                assignFloat(backgroundValue, "parallax", &parallax);
                
                float scale = 1;
                assignFloat(backgroundValue, "scale", &scale);
                
                int x = 0;
                int y = 0;
                assignInt(backgroundValue, "x", &x);
                assignInt(backgroundValue, "y", &y);
                    
                background = new CBackground(spriteKey, parallax, BackgroundOffset{x, y, scale});
                addBackground(i->name.GetString(), background);
            }
            
            background->onDeserialize(backgroundValue, instance);
        }
    }
    
    if(value->HasMember("entities")) {
        const rapidjson::Value& entityValues = (*value)["entities"];

        for(rapidjson::Value::ConstMemberIterator i = entityValues.MemberBegin(); i != entityValues.MemberEnd(); i++) {
            const rapidjson::Value* entityValue = &i->value;
            
            auto entity = getEntity(i->name.GetString());
            if(entity == nullptr) {
                if(entityValue->HasMember("spriteKey"))
                    entity = new CEntity(Box(0, 0, 0, 0), (*entityValue)["spriteKey"].GetString());
                else
                    entity = new CEntity(Box(0, 0, 0, 0), "");
                
                addEntity(entity, i->name.GetString());
            }
            
            entity->onDeserialize(entityValue, instance);
        }
    }
}

void CEntityManager::onEvent(CInstance *instance, int key, bool keyDown) {
    for(auto& i: _entities)
        i.second->onEvent(instance, key, keyDown);
}

void CEntityManager::onKeyStates(CInstance* instance, const Uint8* keystates) {
    for(auto& i: _entities)
        i.second->onKeyStates(instance, keystates);
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