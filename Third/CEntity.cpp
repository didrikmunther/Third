//
//  CEntity.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <math.h>

#include "CEntity.h"
#include "NSurface.h"
#include "CCamera.h"
#include "Define.h"
#include "CEntityManager.h"
#include "CGuiText.h"
#include "CInstance.h"
#include "CChatBubble.h"
#include "CAssetManager.h"

#include <iostream>


CEntity::CEntity(Box rect, Color color) :
spriteKey(""), defaultSprite(""), body(new CBody(rect)), color(color) {
    init();
}

CEntity::CEntity(Box rect, std::string spriteKey) :
spriteKey(spriteKey.c_str()), defaultSprite(spriteKey), body(new CBody(rect)), color(Color{255,0,255,255}) /* sprite not found color */ {
    init();
    //std::fill(spriteStateTypes, spriteStateTypes+SpriteStateTypes::TOTAL_SPRITESTATETYPES, spriteKey);
}

CEntity::~CEntity() {
    _cleanUpTextVector();
    _cleanUpComponents();
    delete body;
}

void CEntity::init() {
    isDead         = false;
    toRemove       = false;
    properties     = EntityProperty::COLLIDABLE | EntityProperty::GRAVITY_AFFECT;
    collisionSides = false;
    collisionLayer = CollisionLayers::LAYER0;
    transparency = 255;
    angle = 0;
}

void CEntity::_cleanUpTextVector() {
    auto i = guiTextVector.begin();
    while(i != guiTextVector.end()) {
        delete *i;
        i = guiTextVector.erase(i);
    }
    guiTextVector.clear();
}

void CEntity::onLoop(CInstance* instance) {
    if(instance->isPaused && instance->player != this && instance->controller != this)
        return;
        
    _hasMoved = false;
    
    auto i = guiTextVector.begin();
    while(i != guiTextVector.end()) {
        (*i)->onLoop();
        if((*i)->toRemove()) {
            delete *i;
            guiTextVector.erase(std::remove(guiTextVector.begin(), guiTextVector.end(), (*i)), guiTextVector.end());
        } else
            ++i;
    }
    
    if(hasProperty(EntityProperty::GRAVITY_AFFECT))
        body->velY += instance->gravity;
    
    if(!hasProperty(EntityProperty::FLIP_FREEZED)) {
        if(body->velX > 0)
            removeProperty(EntityProperty::FLIP);
        else if(body->velX < 0)
            addProperty(EntityProperty::FLIP);
    }
    
    setSprite(getSpriteFromState("IDLE"));
    
    if(body->velY < 0)
        setSprite(getSpriteFromState("ASCENDING"));
    else if(!collisionSides.bottom)
        setSprite(getSpriteFromState("DESCENDING"));
    
    isDead |= body->getY() > 50000 || body->getY() < -50000; // Kill entity if too far down or up
    
    
    for(auto& i: components)
        i.second->onLoop(instance);
}

void CEntity::onEvent(CInstance* instance, int key, bool keyDown) {
    if(instance->player != this && instance->controller != this)
        return;
    
    for(auto& i: components)
        i.second->onEvent(instance, key, keyDown);
}

void CEntity::onKeyStates(CInstance* instance, const Uint8* keystates) {
    if(instance->player != this && instance->controller != this)
        return;
    
    for(auto& i: components)
        i.second->onKeyStates(instance, keystates);
}

void CEntity::onTextInput(CInstance* instance, std::string input) {
    if(instance->player != this && instance->controller != this)
        return;
    
    for(auto& i: components)
        i.second->onTextInput(instance, input);
}

void CEntity::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    
    if(toRemove || isDead)
        return;
    
    int x = body->getX() - camera->offsetX();
    int y = body->getY() - camera->offsetY();
    int w = body->getW();
    int h = body->getH();
    
    if(camera->collision(this) && !(hasProperty(EntityProperty::HIDDEN))) {
        if(!hasSprite())
                NSurface::renderRect(x, y, w, h,
                                     window, color.r, color.g, color.b);
        else {
            getSprite()->onRender(this, Box{x, y, body->getW(), body->getH()}, hasProperty(EntityProperty::FLIP), angle, transparency, window, camera, renderFlags);
        }
    }
    
    for(auto &i: components) {
        i.second->onRender(window, camera, renderFlags);
    }
}

void CEntity::onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance) {
    _serialize(value, alloc);
    
    rapidjson::Value componentValues(rapidjson::kObjectType);
    for(auto& i: components) {
        rapidjson::Value component(rapidjson::kObjectType);
        i.second->onSerialize(&component, alloc, instance);
        componentValues.AddMember(rapidjson::Value(i.second->object.getScript()->getName().c_str(), *alloc), component, *alloc);
    }
    
    value->AddMember("components", componentValues, *alloc);
    
//    if(/* type == SERIALIZE_NEW */ true)
//        removeDuplicates(value, oldSerialized);
//    
//    oldSerialized = value;
    
}

void CEntity::onDeserialize(const rapidjson::Value* value, CInstance* instance) {
    _deserialize(value);
    
    if(!value->HasMember("components"))
        return;
    
    const rapidjson::Value& componentsValues = (*value)["components"];
    
    for(rapidjson::Value::ConstMemberIterator i = componentsValues.MemberBegin(); i != componentsValues.MemberEnd(); i++) {
        const rapidjson::Value* componentValue = &i->value;
        std::string name = i->name.GetString();
        
        auto component = getComponent(name);
        if(component == nullptr) {
            component = addComponent(instance, CAssetManager::getLuaScript(name));
        }
        
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
        componentValue->Accept(writer);
        
        component->onDeserialize(sb.GetString(), instance);
    }
}

void CEntity::_serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc) {
    
    addNumber(value, alloc, "x", body->_rect.x);
    addNumber(value, alloc, "y", body->_rect.y);
    addNumber(value, alloc, "w", body->_rect.w);
    addNumber(value, alloc, "h", body->_rect.h);
    
    addNumber(value, alloc, "velX", body->velX);
    addNumber(value, alloc, "velY", body->velY);
    
    rapidjson::Value sprites(rapidjson::kObjectType);
    for(auto& i: spriteStateTypes) {
        addString(&sprites, alloc, i.first, i.second);
    }
    addValue(value, alloc, "spriteKeys", &sprites);
    
    addString(value, alloc, "defaultSpriteKey", defaultSprite);
    addNumber(value, alloc, "colorR", color.r);
    addNumber(value, alloc, "colorG", color.g);
    addNumber(value, alloc, "colorB", color.b);
    addNumber(value, alloc, "colorA", color.a);
    addNumber(value, alloc, "properties", properties);
    addNumber(value, alloc, "collisionLayers", collisionLayer);

}

void CEntity::_deserialize(const rapidjson::Value* value) {
    
    assignInt(value, "x", &body->_rect.x);
    assignInt(value, "y", &body->_rect.y);
    assignInt(value, "w", &body->_rect.w);
    assignInt(value, "h", &body->_rect.h);
    
    assignFloat(value, "velX", &body->velX);
    assignFloat(value, "velY", &body->velY);
    
    if(value->HasMember("spriteKeys")) {
        for(rapidjson::Value::ConstMemberIterator i = (*value)["spriteKeys"].MemberBegin(); i != (*value)["spriteKeys"].MemberEnd(); i++) {
            const rapidjson::Value* val = &i->value;
            std::string key = val->GetString();
            std::string name = i->name.GetString();
            
            spriteStateTypes[name] = key;
        }
    }
    
    assignString(value, "defaultSpriteKey", &defaultSprite);
    assignInt(value, "properties", &properties);
    assignInt(value, "collisionLayers", &collisionLayer);
    
    assignInt(value, "colorR", &color.r);
    assignInt(value, "colorG", &color.g);
    assignInt(value, "colorB", &color.b);
    assignInt(value, "colorA", &color.a);
    
}

void CEntity::onClick(int x, int y, CInstance* instance) {
    for(auto& i: components) {
        i.second->onClick(x, y, instance);
    }
}

bool CEntity::isOnCollisionLayer(int collisionLayer) {
    return this->collisionLayer & collisionLayer;
}

void CEntity::say(std::string text, std::string fontKey, int type) {
    say(text, fontKey, (ChatBubbleType)type);
}

void CEntity::say(std::string text, std::string fontKey, ChatBubbleType type) {
    CChatBubble* temp = new CChatBubble(text, this, fontKey, (ChatBubbleType)type);
    guiTextVector.push_back(temp);
}

void CEntity::setSpriteOnState(std::string state, std::string sprite) {
    spriteStateTypes[state] = sprite;
}

std::string CEntity::getSpriteFromState(std::string key) {
    if(!spriteStateTypes.count(key))
        return defaultSprite;
    
    return spriteStateTypes[key];
}

bool CEntity::hasProperty(int property) {
    return properties & property;
}

void CEntity::toggleProperty(int property) {
    properties ^= property;
}

void CEntity::addProperty(int property) {
    properties |= property;
}

void CEntity::removeProperty(int property) {
    properties &= ~(property);
}

void CEntity::setSprite(std::string spriteKey) {
    this->spriteKey = spriteKey;
}

CSprite* CEntity::getSprite() {
    return CAssetManager::getSprite(spriteKey);
}

std::string CEntity::getSpriteKey() {
    return spriteKey;
}

bool CEntity::hasSprite() {
    if(getSprite() == nullptr) {
        return false;
    }
    return true;
}

CComponent* CEntity::addComponent(CInstance* instance, CLuaScript* script) {
    if(script == nullptr) {
        NFile::log(LogType::WARNING, "Luascript unknown is null, and could not be added to entity as a component.");
        return nullptr;
    } else if(script->isInvalid()) {
        NFile::log(LogType::WARNING, "Luascript ", script->getName(), " is invalid, and could not be added to entity as a component.");
        return nullptr;
    } else if(getComponent(script->getName())) {
        if(DEBUG)
            NFile::log(LogType::WARNING, "Component already exists: ", script->getName(), ".");
        delete components[script->getName()];
        components.erase(script->getName());
    }
    
    CComponent* component = new CComponent(this, instance, script);
    components[script->getName()] = component;
    component->onInit(instance);
    
    for(auto& i: components) {
        component->onComponentAdd(instance, i.second->object.getScript()->getName());
        i.second->onComponentAdd(instance, script->getName());
    }
    
    auto require = component->object.getTable("require");       // TODO: Add recursion protection, and perhaps a require depth limit.
    for(auto& i: require) {
        std::string name = i.second.cast<std::string>();
        if(getComponent(name)) {
            if(DEBUG)
                NFile::log(LogType::WARNING, "Couldn't add the required component \"", name, "\" to component \"", script->getName(), "\". Because the component already exist.");
            continue;
        }
        auto scriptToAdd = CAssetManager::getLuaScript(name);
        if(!scriptToAdd) {
            NFile::log(LogType::WARNING, "Couldn't add the required component \"", name, "\" to component \"", script->getName(), "\". Because the script doesn't exist.");
        } else {
            CEntity::addComponent(instance, scriptToAdd);
        }
    }
    
    return component;
}

CComponent* CEntity::getComponent(std::string key) {
    if(!components.count(key))
        return nullptr;
    
    return components[key];
}

int CEntity::getComponent(lua_State* L) {
    std::string key = lua_tostring(L, 2);
    
    auto component = getComponent(key);
    if(!component)
        lua_pushnil(L);
    else
        component->pushThis();
    
    return 1;
}

void CEntity::renderAdditional(CWindow *window, CCamera *camera, RenderFlags renderFlags) {
    
    if(renderFlags & RenderFlags::COLLISION_BORDERS && !isDead) {                             // Render collision boxes
        int r, g, b = 0;
        if(hasProperty(EntityProperty::COLLIDABLE)) {r = 255; g = 0; b = 0;  }
        else                                        {r = 0; g = 255; b = 255;}
        
        NSurface::renderRect(body->getX() - camera->offsetX(), body->getY() - camera->offsetY(), 1, body->getH() - 1,window, r, g, b);    // Left line
        NSurface::renderRect(body->getX() - camera->offsetX(), body->getY() - camera->offsetY(), body->getW() - 1, 1, window, r, g, b);      // Top line
        NSurface::renderRect(body->getX() + body->getW() - camera->offsetX() - 1, body->getY() - camera->offsetY(), 1, body->getH(), window, r, g, b);  // Right line
        NSurface::renderRect(body->getX() - camera->offsetX(), body->getY() + body->getH() - camera->offsetY() - 1, body->getW(), 1, window, r, g, b);  // Bosttom line
    }
        
    for(auto &i: components) {
        i.second->onRenderAdditional(window, camera, renderFlags);
    }
    
    if(!hasProperty(EntityProperty::HIDDEN)) {
        for (auto &i: guiTextVector) {
            i->onRender(window, camera, renderFlags);
        }
    }
    
}

bool CEntity::coordinateCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2) {
    if(x + 1 > x2 + w2)
        return false;
    if(x - 1 + w < x2)
        return false;
    if(y + 1 > y2 + h2)
        return false;
    if(y - 1 + h < y2)
        return false;
    
    return true;
}

bool CEntity::coordinateCollision(int x, int y, int w, int h) {
    return coordinateCollision(x, y, w, h, body->getX(), body->getY(), body->getW(), body->getH());
}

bool CEntity::_collision(int x, int y, std::vector<CEntity*>* entities) {
    
    if(!hasProperty(EntityProperty::COLLIDABLE)) return false;
    
    bool colliding = false;
    
    for (auto &target: *entities) {
        
        if (target == this) continue;
        if (!(target->properties & EntityProperty::COLLIDABLE)) continue;
        if (target->isDead) continue;
        if (!target->isOnCollisionLayer(collisionLayer)) continue;
    
        if(!coordinateCollision(x, y, body->getW(), body->getH(),
                                target->body->getX(), target->body->getY(), target->body->getW(), target->body->getH()))
            continue;
        
        CollisionSides tmpCollisionSides;
        
        if(y - 1 + body->getH() <= target->body->getY())
            tmpCollisionSides.bottom = true;
        if(y + 1 == target->body->getY() + target->body->getH())
            tmpCollisionSides.top = true;
        if(x + 1 >= target->body->getX() + target->body->getW())
            tmpCollisionSides.left = true;
        if(x + body->getW() - 1 <= target->body->getX())
            tmpCollisionSides.right = true;
        
        colliding = _onCollision(target, &tmpCollisionSides);
        collisionSides |= tmpCollisionSides;
    }
    
    if(colliding) {
//        std::string toWrite = "";           // Write where the entity is colliding
//        if(collisionTop)
//            toWrite += "Top, ";
//        if(collisionBottom)
//            toWrite += "Bot, ";
//        if(collisionLeft)
//            toWrite += "Left, ";
//        if(collisionRight)
//            toWrite += "Right, ";
//        say(toWrite, "TESTFONT", ChatBubbleType::INSTANT_TALK);
        return true;
    } else
        return false;
}

void CEntity::move(std::vector<CEntity*>* entities, CInstance* instance) {
    if(instance->isPaused && instance->player != this && instance->controller != this)
        return;
    
    if(hasProperty(EntityProperty::STATIC) || isDead) {
        body->velY = body->velX = 0;
        return;
    }
    
    int MoveX, MoveY;
    
    if(instance->gravity < 0.5) {         // Check if gravity can be rounded up, otherwise wonkyness happens
        MoveX = ceil(body->velX);
        MoveY = ceil(body->velY);
    } else if(instance->gravity != 0) {
        MoveX = round(body->velX);
        MoveY = round(body->velY);
    } else {
        MoveX =
        MoveY = 0;
    }
    
    int StopX = body->getX();
    int StopY = body->getY();
    
    int NewX = 0;
    int NewY = 0;
    
    if(MoveX != 0) {
        if(MoveX >= 0) 	NewX =  1;
        else 			NewX = -1;
    }
    
    if(MoveY != 0) {
        if(MoveY >= 0) 	NewY =  1;
        else 			NewY = -1;
    }
    
    collisionSides = false;
    
    while(true) {
        if(!_collision(StopX + NewX, StopY, entities)) {
            StopX += NewX;
            body->_rect.x += NewX;
        } else {
            body->velX = 0;
        }
        
        if(!_collision(StopX, StopY + NewY, entities)) {
            StopY += NewY;
            body->_rect.y += NewY;
        } else {
            body->velY = 0;
        }
        
        MoveX += -NewX;
        MoveY += -NewY;
        
        if(NewX > 0 && MoveX <= 0) NewX = 0;
        if(NewX < 0 && MoveX >= 0) NewX = 0;
        
        if(NewY > 0 && MoveY <= 0) NewY = 0;
        if(NewY < 0 && MoveY >= 0) NewY = 0;
        
        if(MoveX == 0 && MoveY == 0) 	break;
        if(NewX == 0 && NewY == 0) 		break;
    }
    
    _hasMoved = !(body->_rect == body->_previousRect);
    body->onLoop();
}

bool CEntity::_onCollision(CEntity *target, CollisionSides* collisionSides) {
    bool toReturn = true;
    
    for(auto &i: components) {
        bool ans = i.second->onCollision(target, collisionSides);
        toReturn = toReturn && ans;
    }
    
    return toReturn;
}

void CEntity::_cleanUpComponents() {
    auto i = components.begin();
    while(i != components.end()) {
        delete i->second;
        components.erase(i++->first);
    }
    components.clear();
}

