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
#include "CChatBubble.h"
#include "CEntityManager.h"

CEntity::CEntity(sf::IntRect rect, sf::Color color) :
    /*sprite(nullptr),*/ spriteKey(""), assetManager(nullptr), body(rect), color(color) {
        initValues();
}

CEntity::CEntity(sf::IntRect rect, std::string spriteKey, CAssetManager* assetManager) :
/*sprite(sprite),*/ spriteKey(spriteKey), assetManager(assetManager), body(rect), color(sf::Color{255,0,255,255}) {
        initValues();
}

void CEntity::initValues() {
    toRemove            = false;
    properties          = EntityProperty::COLLIDABLE;
    collisionTop        = false;
    collisionBottom     = false;
    collisionRight      = false;
    collisionLeft       = false;
    flip                = false;
}

void CEntity::onLoop(std::map<std::string, CEntity*>* entities) {
    
    if(!hasProperty(EntityProperty::FLYING))
        body.velY += GRAVITY;
    
    _doLogic();
    if(!hasProperty(EntityProperty::STATIC))
        move(entities);
    else
        body.velY = body.velX = 0;
}

void CEntity::onRender(sf::RenderWindow* window, CCamera* camera, int renderFlags) {
    if(camera->collision(this) && !(hasProperty(EntityProperty::HIDDEN))) {
        //std::cout << sprite << std::endl;
        if(assetManager == nullptr || assetManager->getSprite(spriteKey) == nullptr)
                NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(),
                                     body.getW(), body.getH(),
                                     window, color.r, color.g, color.b);
        else
            //NSurface::renderSprite(assetManager->getSprite(spriteKey), renderer, SDL_Rect{body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.rect.w, body.rect.h});
            NSurface::renderSprite(assetManager->getSprite(spriteKey), window, sf::IntRect{body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.getW(), body.getH()});
    }
    
    if(renderFlags & RenderFlags::COLLISION_BORDERS) {
        int r, g, b = 0;
        if(hasProperty(EntityProperty::COLLIDABLE)) {r = 255; g = 0; b = 0;  }
        else                                        {r = 0; g = 255; b = 255;}
        
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), 1, body.getH() - 1, window, r, g, b);    // Left line
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.getW() - 1, 1, window, r, g, b);      // Top line
        NSurface::renderRect(body.getX() + body.getW() - camera->offsetX() - 1, body.getY() - camera->offsetY(), 1, body.getH(), window, r, g, b);  // Right line
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() + body.getH() - camera->offsetY() - 1, body.getW(), 1, window, r, g, b);  // Bottom line
    }
}

void CEntity::say(std::string text, std::string fontKey, CAssetManager* assetManager, CEntityManager* entityManager, int type) {
    CChatBubble* temp = new CChatBubble(text, this, fontKey, assetManager, type);
    entityManager->addGuiText(temp);
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
    
        if(x + 1 > i.second->body.getX() + i.second->body.getW())
            continue;
        if(x - 1 + body.getW() < i.second->body.getX())
            continue;
        if(y + 1 > i.second->body.getY() + i.second->body.getH())
            continue;
        if(y - 1 + body.getH() < i.second->body.getY())
            continue;
        
        if(y - 1 + body.getH() <= i.second->body.getY() && x + body.getW() - 1 > i.second->body.getX() && x + 1 < i.second->body.getX() + i.second->body.getW())
            collisionBottom = true;
        if(y - 1 >= i.second->body.getY() + i.second->body.getH())
            collisionTop = true;
        if(x + 1> i.second->body.getX() && y + body.getH() > i.second->body.getY() && y < i.second->body.getY() + i.second->body.getH())
            collisionLeft = true;
        if(x + 1 < i.second->body.getX() && y + body.getH() > i.second->body.getY())
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
            body.rect.left += NewX;
        } else {
            body.velX = 0;
        }
        
        if(!collision(StopX, StopY + NewY, entities)) {
            StopY += NewY;
            body.rect.top += NewY;
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

void CEntity::_doLogic() {
    //if(body.rect.y > DESPAWN_HEIGHT)
        //toRemove = true;
}

