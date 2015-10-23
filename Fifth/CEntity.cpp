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
#include "CSpriteContainer.h"
#include "CGuiText.h"
#include "CGlobalSettings.h"
#include "CInstance.h"
#include "CChatBubble.h"

#include <iostream>


CEntity::CEntity(Box rect, Color color) :
spriteContainerKey(""), body(new CBody(rect)), color(color) {
    init();
}

CEntity::CEntity(Box rect, std::string spriteContainerKey) :
spriteContainerKey(spriteContainerKey), body(new CBody(rect)), color(SDL_Color{255,0,255,255}) /* sprite not found color */ {
    init();
    std::fill(spriteStateTypes, spriteStateTypes+SpriteStateTypes::TOTAL_SPRITESTATETYPES, spriteContainerKey);
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
    spriteFollowsCollisionBox = true;
    transparency = 255;
}

void CEntity::_cleanUpTextVector() {
    auto i = _GuiTextVector.begin();
    while(i != _GuiTextVector.end()) {
        delete *i;
        i = _GuiTextVector.erase(i);
    }
    _GuiTextVector.clear();
}

void CEntity::onLoop(CInstance* instance) {
    _hasMoved = false;
    
    auto i = _GuiTextVector.begin();
    while(i != _GuiTextVector.end()) {
        (*i)->onLoop();
        if((*i)->toRemove()) {
            delete *i;
            _GuiTextVector.erase(std::remove(_GuiTextVector.begin(), _GuiTextVector.end(), (*i)), _GuiTextVector.end());
        } else
            ++i;
    }
    
    if(hasProperty(EntityProperty::GRAVITY_AFFECT))
        body->velY += CGlobalSettings::GRAVITY;
    
    if(!hasProperty(EntityProperty::FLIP_FREEZED)) {
        if(body->velX > 0)
            removeProperty(EntityProperty::FLIP);
        else if(body->velX < 0)
            addProperty(EntityProperty::FLIP);
    }
    
    setSpriteContainer(spriteStateTypes[SpriteStateTypes::IDLE]);
    
    if(body->velY < 0)
        setSpriteContainer(spriteStateTypes[SpriteStateTypes::ASCENDING]);
    else if(!collisionSides.bottom)
        setSpriteContainer(spriteStateTypes[SpriteStateTypes::DESCENDING]);
    
    isDead |= body->getY() > 50000 || body->getY() < -50000; // Kill entity if too far down or up
    
    
    for(auto& i: components)
        i.second->onLoop(instance);
    
    // --
    
//    rapidjson::Document d;
//    d.Parse("{}");
//    
//    rapidjson::Value entityValue(rapidjson::kObjectType);
//    serialize(&entityValue, &d.GetAllocator());
//    
//    d.AddMember("this", entityValue, d.GetAllocator());
//    
//    rapidjson::StringBuffer sb;
//    rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
//    d.Accept(writer);
    
//    if(!isDead && !getComponent<EParticle>())
//        say(sb.GetString(), "TESTFONT", ChatBubbleType::INSTANT_TALK);
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
            int spriteWidth, spriteHeight = 0;
            if(spriteFollowsCollisionBox) {
                spriteWidth = body->getW();
                spriteHeight = body->getH();
            } else {
                spriteWidth = getSpriteContainer()->spriteArea.w;
                spriteHeight = getSpriteContainer()->spriteArea.h;
            }
            
            SDL_RendererFlip flip;
            if(hasProperty(EntityProperty::FLIP))
                flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
            else
                flip = SDL_RendererFlip::SDL_FLIP_NONE;
            
            NSurface::renderSprite(x, y, spriteWidth, spriteHeight, getSpriteContainer()->getSprite(), window, flip, transparency);
        }
    }
    
    for(auto &i: components) {
        i.second->onRender(window, camera, renderFlags);
    }
}

void CEntity::serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc) {
    
//    rapidjson::Value componentValues(rapidjson::kObjectType);
//    for(auto& i: components) {
//        rapidjson::Value component(rapidjson::kObjectType);
//        i.second->serialize(&component, alloc);
//        componentValues.AddMember(rapidjson::Value(i.second->type.c_str(), *alloc), component, *alloc);
//    }
//    
//    value->AddMember("components", componentValues, *alloc);
    
}

void CEntity::deserialize(rapidjson::Value* value) {
    
    for(auto& i: components) {
        i.second->deserialize(value);
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
    _GuiTextVector.push_back(temp);
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

void CEntity::setSpriteContainer(std::string spriteContainerKey) {
    this->spriteContainerKey = spriteContainerKey;
}

CSpriteContainer* CEntity::getSpriteContainer() {
    return CAssetManager::getSpriteContainer(spriteContainerKey);
}

std::string CEntity::getSpriteContainerKey() {
    return spriteContainerKey;
}

bool CEntity::hasSprite() {
    if(getSpriteContainer() == nullptr || getSpriteContainer()->getSprite() == nullptr) {
        return false;
    }
    return true;
}

void CEntity::shoot(float angle, BasicUtilities basicUtility) {
    
//    int precision = 20;
//    int spread = 200;
//    
//    angle += (rand() % precision - precision / 2) / (float)spread;
//    
//    float velocity = 20;
//    float velX = cos(angle) * (velocity);
//    float velY = sin(angle) * (velocity);
    
//    CEntity* tempParticle = new CEntity(Box{body->getX(), body->getY() - 100, 8, 8}, SDL_Color{(Uint8)(rand() % 255), (Uint8)(rand() % 255), (Uint8)(rand() % 255), 0});
//    tempParticle->addComponent<EParticle>(10);
//    EUtility* utilityComponent = new EUtility(tempParticle, this, basicUtility);
//    tempParticle->addComponent<EUtility>(utilityComponent);
//    tempParticle->body->velX = velX + body->velX;
//    tempParticle->body->velY = velY + body->velY;
//    particlesToAdd.push_back(tempParticle);
}

void CEntity::addComponent(CLuaScript* script) {
    if(getComponent(script->getName())) {
        NFile::log(LogType::WARNING, "Component already exists: ", script->getName(), ".");
        delete components[script->getName()];
    }
    
    CComponent* component = new CComponent(this, script);
    components[script->getName()] = component;
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
    
    if(!hasProperty(EntityProperty::HIDDEN))
        for (auto &i: _GuiTextVector)                                                // Render chatbubbles
            i->onRender(window, camera, renderFlags);
    
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

void CEntity::move(std::vector<CEntity*>* entities) {
    
    if(hasProperty(EntityProperty::STATIC) || isDead) {
        body->velY = body->velX = 0;
        return;
    }
    
    int MoveX, MoveY;
    
    if(CGlobalSettings::GRAVITY < 0.5) {         // Check if gravity can be rounded up, otherwise wonkyness happens
        MoveX = ceil(body->velX);
        MoveY = ceil(body->velY);
    } else if(CGlobalSettings::GRAVITY != 0) {
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

