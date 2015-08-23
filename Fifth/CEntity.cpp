//
//  CEntity.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CEntity.h"
#include "NSurface.h"
#include <iostream>
#include "CCamera.h"
#include "Define.h"
#include "CEntityManager.h"
#include "CLiving.h"
#include "CSpriteContainer.h"
#include <math.h>

CEntity::CEntity(Box rect, SDL_Color color) :
spriteContainerKey(""), CCollidable(rect), color(color) {
    init();
}

CEntity::CEntity(Box rect, std::string spriteContainerKey) :
spriteContainerKey(spriteContainerKey), CCollidable(rect), color(SDL_Color{255,0,255,255}) /* sprite not found color */ {
    init();
    std::fill(spriteStateTypes, spriteStateTypes+SpriteStateTypes::TOTAL_SPRITESTATETYPES, spriteContainerKey);
    
}

CEntity::~CEntity() {
    _cleanUpTextVector();
}

void CEntity::init() {
    entityType = EntityTypes::Entity;
    
    _isDead             = false;
    _toRemove           = false;
    properties          = EntityProperty::COLLIDABLE;
    collisionTop        = false;
    collisionBottom     = false;
    collisionRight      = false;
    collisionLeft       = false;
    collisionLayer = CollisionLayers::LAYER0;
}

void CEntity::_cleanUpTextVector() {
    auto i = _GuiTextVector.begin();
    while(i != _GuiTextVector.end()) {
        delete *i;
        i = _GuiTextVector.erase(i);
    }
    _GuiTextVector.clear();
}

void CEntity::onLoop() {
    
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
    
    if(!hasProperty(EntityProperty::FLYING))
        body.velY += GRAVITY;
    
    _doLogic();
}

void CEntity::afterLogicLoop(std::vector<CEntity *> *entities) {
    if(isDead())
        return;
    
    if(!hasProperty(EntityProperty::STATIC))
        move(entities);
    else
        body.velY = body.velX = 0;
    
    body.onLoop();
}

void CEntity::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    
    if(toRemove() || isDead())
        return;
    
    int x = body.getX() - camera->offsetX();
    int y = body.getY() - camera->offsetY();
    int w = body.getW();
    int h = body.getH();
    
    if(camera->collision(this) && !(hasProperty(EntityProperty::HIDDEN))) {
        if(!hasSprite())
                NSurface::renderRect(x, y, w, h,
                                     window, color.r, color.g, color.b);
        else {
            int spriteWidth, spriteHeight = 0;
            if(spriteFollowsCollisionBox) {
                spriteWidth = body.getW();
                spriteHeight = body.getH();
            } else {
                spriteWidth = getSpriteContainer()->spriteArea.w;
                spriteHeight = getSpriteContainer()->spriteArea.h;
            }
            
            SDL_RendererFlip flip;
            if(hasProperty(EntityProperty::FLIP))
                flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
            else
                flip = SDL_RendererFlip::SDL_FLIP_NONE;
            
            NSurface::renderSprite(x, y, spriteWidth, spriteHeight, getSpriteContainer()->getSprite(), window, flip, getTransparency());
        }
    }
}

bool CEntity::isOnCollisionLayer(int collisionLayer) {
    return this->collisionLayer & collisionLayer;
}

void CEntity::say(std::string text, std::string fontKey, ChatBubbleType type) {
    CChatBubble* temp = new CChatBubble(text, this, fontKey, type);
    _GuiTextVector.push_back(temp);
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

void CEntity::renderAdditional(CWindow *window, CCamera *camera, RenderFlags renderFlags) {
    
    if(renderFlags & RenderFlags::COLLISION_BORDERS && !isDead()) {                             // Render collision boxes
        int r, g, b = 0;
        if(hasProperty(EntityProperty::COLLIDABLE)) {r = 255; g = 0; b = 0;  }
        else                                        {r = 0; g = 255; b = 255;}
        
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), 1, body.getH() - 1,window, r, g, b);    // Left line
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.getW() - 1, 1, window, r, g, b);      // Top line
        NSurface::renderRect(body.getX() + body.getW() - camera->offsetX() - 1, body.getY() - camera->offsetY(), 1, body.getH(), window, r, g, b);  // Right line
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() + body.getH() - camera->offsetY() - 1, body.getW(), 1, window, r, g, b);  // Bottom line
    }
    
    if(!hasProperty(EntityProperty::HIDDEN))
        for (auto &i: _GuiTextVector)                                                // Render chatbubbles
            i->onRender(window, camera);
        
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
    return coordinateCollision(x, y, w, h, body.getX(), body.getY(), body.getW(), body.getH());
}

bool CEntity::_collision(int x, int y, std::vector<CEntity*>* entities) {
    
    if(!hasProperty(EntityProperty::COLLIDABLE)) return false;
    
    bool colliding = false;
    
    for (auto &target: *entities) {
        
        if (target == this) continue;
        if (!(target->properties & EntityProperty::COLLIDABLE)) continue;
        if (target->isDead()) continue;
        if (!target->isOnCollisionLayer(collisionLayer)) continue;
    
        if(!coordinateCollision(x, y, body.getW(), body.getH(),
                                target->body.getX(), target->body.getY(), target->body.getW(), target->body.getH()))
            continue;
        
        bool tmpCollisionBottom = false,
             tmpCollisionTop = false,
             tmpCollisionLeft = false,
             tmpCollisionRight = false;
        
        if(y - 1 + body.getH() <= target->body.getY())
            tmpCollisionBottom = true;
        if(y + 1 == target->body.getY() + target->body.getH())
            tmpCollisionTop = true;
        if(x + 1 >= target->body.getX() + target->body.getW())
            tmpCollisionLeft = true;
        if(x + body.getW() - 1 <= target->body.getX())
            tmpCollisionRight = true;
        
        colliding = _collisionLogic(target, CollisionSides{tmpCollisionTop, tmpCollisionBottom, tmpCollisionRight, tmpCollisionLeft});
        collisionBottom |= tmpCollisionBottom;
        collisionTop    |= tmpCollisionTop;
        collisionRight  |= tmpCollisionRight;
        collisionLeft   |= tmpCollisionLeft;
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
    
    int MoveX, MoveY;
    
    if(GRAVITY < 0.5) {         // Check if gravity can be rounded up, otherwise wonkyness happens
        MoveX = ceil(body.velX);
        MoveY = ceil(body.velY);
    } else {
        MoveX = round(body.velX);
        MoveY = round(body.velY);
    }
    
    int StopX = body.getX();
    int StopY = body.getY();
    
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
    
    collisionLeft = collisionRight = false;
    collisionTop = collisionBottom = false;
    
    while(true) {
        if(!_collision(StopX + NewX, StopY, entities)) {
            StopX += NewX;
            body._rect.x += NewX;
        } else {
            body.velX = 0;
        }
        
        if(!_collision(StopX, StopY + NewY, entities)) {
            StopY += NewY;
            body._rect.y += NewY;
        } else {
            body.velY = 0;
        }
        
        MoveX += -NewX;
        MoveY += -NewY;
        
        if(NewX > 0 && MoveX <= 0) NewX = 0;
        if(NewX < 0 && MoveX >= 0) NewX = 0;
        
        if(NewY > 0 && MoveY <= 0) NewY = 0;
        if(NewY < 0 && MoveY >= 0) NewY = 0;

//        if(MoveX == 0) NewX = 0;
//        if(MoveY == 0) NewY = 0;
        
        if(MoveX == 0 && MoveY == 0) 	break;
        if(NewX == 0 && NewY == 0) 		break;
    }
    
    _hasMoved = !(body._rect == body._previousRect);
}

//void CEntity::move(std::map<std::string, CEntity*>* entities) {
//    
//    int MoveX = round(body.velX);
//    int MoveY = round(body.velY);
//    
//    int StopX = body.getX();
//    int StopY = body.getY();
//    
//    int NewX = 0;
//    int NewY = 0;
//    
//    if(MoveX != 0) {
//        if(MoveX >= 0) 	NewX =  1;
//        else 			NewX = -1;
//    }
//    
//    if(MoveY != 0) {
//        if(MoveY >= 0) 	NewY =  1;
//        else 			NewY = -1;
//    }
//    
//    collisionLeft = collisionRight = false;
//    collisionTop = collisionBottom = false;
//    
//    do {
//        MoveX -= NewX;
//        if(MoveX == 0) break;
//    } while(_collision(StopX + MoveX, StopY, entities));
//    body.rect.x = StopX + MoveX;
//    
//    do {
//        MoveY -= NewY;
//        if(MoveY == 0) break;
//    } while(_collision(StopX, StopY + MoveY, entities));
//    body.rect.y = StopY + MoveY;
//}

bool CEntity::_collisionLogic(CEntity *target, CollisionSides collisionSides) {
    return true;
}

void CEntity::_doLogic() {
    if(!hasProperty(EntityProperty::FLIP_FREEZED)) {
        if(body.velX > 0)
            removeProperty(EntityProperty::FLIP);
        else if(body.velX < 0)
            addProperty(EntityProperty::FLIP);
    }
    
    setSpriteContainer(spriteStateTypes[SpriteStateTypes::IDLE]);
    
    if(body.velY < 0)
        setSpriteContainer(spriteStateTypes[SpriteStateTypes::ASCENDING]);
    else if(!collisionBottom)
        setSpriteContainer(spriteStateTypes[SpriteStateTypes::DESCENDING]);
}

