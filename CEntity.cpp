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

CEntity::CEntity(SDL_Rect rect, SDL_Color color) :
    /*sprite(nullptr),*/ spriteKey(""), assetManager(nullptr), body(rect), color(color),
    toRemove(false), properties(EntityProperty::COLLIDABLE),
    collisionTop(false), collisionBottom(false),
    collisionRight(false), collisionLeft(false) {
}

CEntity::CEntity(SDL_Rect rect, std::string spriteKey, CAssetManager* assetManager) :
    /*sprite(sprite),*/ spriteKey(spriteKey), assetManager(assetManager), body(rect), color(SDL_Color{255,0,255,0}),
    toRemove(false), properties(EntityProperty::COLLIDABLE),
    collisionTop(false), collisionBottom(false),
    collisionRight(false), collisionLeft(false) {
}

void CEntity::onLoop(std::map<std::string, CEntity*>* entities) {
    
    if(!hasProperty(EntityProperty::FLYING))
        body.velY += GRAVITY;
    
    doLogic();
    if(!hasProperty(EntityProperty::STATIC))
        move(entities);
    else
        body.velY = body.velX = 0;
}

void CEntity::onRender(SDL_Renderer *renderer, CCamera* camera) {
    if(camera->collision(this) && !(hasProperty(EntityProperty::HIDDEN))) {
        //std::cout << sprite << std::endl;
        if(assetManager == nullptr || assetManager->getSprite(spriteKey) == nullptr)
                NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(),
                                     body.getWidth(), body.getHeight(),
                                     renderer, color.r, color.g, color.b);
        else
            NSurface::renderSprite(assetManager->getSprite(spriteKey), renderer, SDL_Rect{body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.rect.w, body.rect.h});
    }
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
    if(hasProperty(property)) toggleProperty(property);
}

int CEntity::setSprite(std::string spriteKey) {
    if(assetManager == nullptr)
        return -1;
    else
        this->spriteKey = spriteKey;
    return 0;
}

bool CEntity::collision(int x, int y, std::map<std::string, CEntity*>* entities) {
    
    if(!(properties & EntityProperty::COLLIDABLE)) return false;
    
    for (auto &i: *entities) {
        
        if (i.second == this) continue;
        if (!(i.second->properties & EntityProperty::COLLIDABLE)) continue;
    
        if(x > i.second->body.getX() + i.second->body.getWidth())
            continue;
        if(x + body.getWidth() < i.second->body.getX())
            continue;
        if(y > i.second->body.getY() + i.second->body.getHeight())
            continue;
        if(y + body.getHeight() < i.second->body.getY())
            continue;
        
        if(y + body.getHeight() <= i.second->body.getY() && x + body.getWidth() > i.second->body.getX() && x < i.second->body.getX() + i.second->body.getWidth())
            collisionBottom = true;
        if(y >= i.second->body.getY() + i.second->body.getHeight())
            collisionTop = true;
        if(x > i.second->body.getX() && y + body.getHeight() > i.second->body.getY() && y < i.second->body.getY() + i.second->body.getHeight())
            collisionLeft = true;
        if(x < i.second->body.getX() && y + body.getHeight() > i.second->body.getY())
            collisionRight = true;
        return true;
    }
    
    return false;
}

void CEntity::move(std::map<std::string, CEntity*>* entities) {
    
    int MoveX = body.velX;
    int MoveY = body.velY;
    
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
        if(!collision(StopX + NewX, StopY, entities)) {
            StopX += NewX;
            body.rect.x += NewX;
        } else {
            body.velX = 0;
        }
        
        if(!collision(StopX, StopY + NewY, entities)) {
            StopY += NewY;
            body.rect.y += NewY;
        } else {
            body.velY = 0;
        }
        
        MoveX += -NewX;
        MoveY += -NewY;
        
        if(NewX > 0 && MoveX <= 0) NewX = 0;
        if(NewX < 0 && MoveX >= 0) NewX = 0;
        
        if(NewY > 0 && MoveY <= 0) NewY = 0;
        if(NewY < 0 && MoveY >= 0) NewY = 0;
        
        if(MoveX == 0) NewX = 0;
        if(MoveY == 0) NewY = 0;
        
        if(MoveX == 0 && MoveY == 0) 	break;
        if(NewX == 0 && NewY == 0) 		break;
    }
    
}

void CEntity::doLogic() {
    //if(body.rect.y > DESPAWN_HEIGHT)
        //toRemove = true;
}

