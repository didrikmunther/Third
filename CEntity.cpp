//
//  CEntity.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CEntity.h"
#include "CSurface.h"
#include <iostream>
#include "CCamera.h"
#include "Define.h"

CEntity::CEntity(SDL_Rect rect, SDL_Color color) :
body(rect), color(color), toRemove(false), properties(EntityProperty::COLLIDABLE) {
}

void CEntity::onLoop(std::map<int, CEntity*>* entities) {
    
    if(!hasProperty(EntityProperty::FLYING))
        body.velY += GRAVITY;
    
    doLogic();
    move(entities);
}

void CEntity::onRender(SDL_Renderer *renderer, CCamera* camera) {
    if(camera->collision(this) && !(hasProperty(EntityProperty::HIDDEN)))
        CSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(),
                         body.getWidth(), body.getHeight(),
                         renderer, color.r, color.g, color.b);
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
    properties = ~(properties & property);
}

bool CEntity::collision(int x, int y, std::map<int, CEntity*>* entities) {
    
    if(!(properties & EntityProperty::COLLIDABLE)) return false;
    
    for (auto &i: *entities) {
        if (i.second == this) continue;
        if (!(i.second->properties & EntityProperty::COLLIDABLE)) continue;
    
        if(x > (i.second->body.getX() + i.second->body.getWidth()))
            continue;
        if(x + body.getWidth() < i.second->body.getX())
            continue;
        if(y > i.second->body.getY() + i.second->body.getHeight())
            continue;
        if(y + body.getHeight() < i.second->body.getY())
            continue;
        else
            return true;
    }
    
    return false;
}

void CEntity::move(std::map<int, CEntity*>* entities) {
    
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
    if(body.rect.y > DESPAWN_HEIGHT)
        toRemove = true;
}

