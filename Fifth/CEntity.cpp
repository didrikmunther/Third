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
#include "CLiving.h"
#include <math.h>

CEntity::CEntity(Box rect, sf::Color color) :
    spriteKey(""), CCollidable(rect), color(color) {
        initValues();
}

CEntity::CEntity(Box rect, std::string spriteKey) :
/*sprite(sprite),*/ spriteKey(spriteKey), CCollidable(rect), color(sf::Color{255,0,255,255}) {
        initValues();
}

CEntity::~CEntity() {
    _cleanUpTextVector();
}

void CEntity::initValues() {
    _isDead             = false;
    toRemove            = false;
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

void CEntity::onLoop(std::map<std::string, CEntity*>* entities) {
    
    auto i = _GuiTextVector.begin();
    while(i != _GuiTextVector.end()) {
        (*i)->onLoop();
        if((*i)->toRemove) {
            delete *i;
            _GuiTextVector.erase(std::remove(_GuiTextVector.begin(), _GuiTextVector.end(), (*i)), _GuiTextVector.end());
        } else
            ++i;
    }
    
    if(isDead())
        return;
    
    if(!hasProperty(EntityProperty::FLYING))
        body.velY += GRAVITY;
    
    _doLogic();
    
    if(!hasProperty(EntityProperty::STATIC))
        move(entities);
    else
        body.velY = body.velX = 0;

}

void CEntity::onRender(CWindow* window, CCamera* camera, int renderFlags) {
    
    if(toRemove || isDead())
        return;
    
    if(camera->collision(this) && !(hasProperty(EntityProperty::HIDDEN))) {
        //std::cout << sprite << std::endl;
        if(CAssetManager::getSprite(spriteKey) == nullptr)
                NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(),
                                     body.getW(), body.getH(),
                                     *window->getRenderTexture(), color.r, color.g, color.b);
        else
            //NSurface::renderSprite(assetManager->getSprite(spriteKey), renderer, SDL_Rect{body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.rect.w, body.rect.h});
            NSurface::renderEntity(this, window, sf::IntRect{body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.getW(), body.getH()}, properties);
    }
}

bool CEntity::isOnCollisionLayer(int collisionLayer) {
    return this->collisionLayer & collisionLayer;
}

void CEntity::say(std::string text, std::string fontKey, int type) {
    CChatBubble* temp = new CChatBubble(text, this, fontKey, type);
    _GuiTextVector.push_back(temp);
}

void CEntity::renderAdditional(CWindow *window, CCamera *camera, int renderFlags) {
    
    if(toRemove || isDead())
        return;
    
    if(renderFlags & RenderFlags::COLLISION_BORDERS) {                             // Render collision boxes
        int r, g, b = 0;
        if(hasProperty(EntityProperty::COLLIDABLE)) {r = 255; g = 0; b = 0;  }
        else                                        {r = 0; g = 255; b = 255;}
        
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), 1, body.getH() - 1, *window->getRenderTexture(), r, g, b);    // Left line
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(), body.getW() - 1, 1, *window->getRenderTexture(), r, g, b);      // Top line
        NSurface::renderRect(body.getX() + body.getW() - camera->offsetX() - 1, body.getY() - camera->offsetY(), 1, body.getH(), *window->getRenderTexture(), r, g, b);  // Right line
        NSurface::renderRect(body.getX() - camera->offsetX(), body.getY() + body.getH() - camera->offsetY() - 1, body.getW(), 1, *window->getRenderTexture(), r, g, b);  // Bottom line
    }
    
    for (auto &i: _GuiTextVector)                                                // Render chatbubbles
        i->onRender(window, camera);
    
    CLiving* living = dynamic_cast<CLiving*>(this);   // If a living entity, call the function for CLiving
    if(living != nullptr) {
        living->cLivingRender(window, camera);
    }
    
}

void CEntity::setSprite(std::string spriteKey) {
    this->spriteKey = spriteKey;
}

CSprite* CEntity::getSprite() {
    return CAssetManager::getSprite(spriteKey);
}

bool CEntity::isDead() {
    return _isDead;
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

bool CEntity::_collision(int x, int y, std::map<std::string, CEntity*>* entities) {
    
    if(!hasProperty(EntityProperty::COLLIDABLE)) return false;
    
    bool colliding = false;
    
    for (auto &i: *entities) {
        
        auto target = i.second;
        
        if (target == this) continue;
        if (!(target->properties & EntityProperty::COLLIDABLE)) continue;
        if (target->isDead()) return;
        //if (!i.second->isOnCollisionLayer(collisionLayer)) return;        // Isn't working right now
    
        if(!coordinateCollision(x, y, body.getW(), body.getH(),
                                target->body.getX(), target->body.getY(), target->body.getW(), target->body.getH()))
            continue;
        
        if(y - 1 + body.getH() <= target->body.getY() &&
           x + body.getW() - 1 > target->body.getX() &&
           x + 1 < target->body.getX() + target->body.getW())
            collisionBottom = true;
        if(y + 1 == target->body.getY() + target->body.getH())
            collisionTop = true;
        if(x + 1 >= target->body.getX() + target->body.getW())
            collisionLeft = true;
        if(x + body.getW() - 1 <= target->body.getX())
            collisionRight = true;
        
        colliding = true;
        _collisionLogic(target);
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

void CEntity::move(std::map<std::string, CEntity*>* entities) {
    
    int MoveX = round(body.velX);
    int MoveY = round(body.velY);
    
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
            body.rect.x += NewX;
        } else {
            body.velX = 0;
        }
        
        if(!_collision(StopX, StopY + NewY, entities)) {
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

void CEntity::_collisionLogic(CEntity *target) {
    
//    float momentumX = (float)body.getW() * body.getH() * body.velX;
//    float targetMomentumX = (float)target->body.getW() * target->body.getH() * target->body.velX;
//    
//    float finalMomentumX = momentumX + targetMomentumX;
//    float finalVelocityX = finalMomentumX / (body.getW() + body.getH() + target->body.getW() + target->body.getH()) / 100;
//    
//    //body.velX += finalVelocityX;
//    target->body.velX += finalVelocityX;
    
//    float weight = (body.getW()*body.getH()) / 10000.0f;
//    
//    float otherWeight = ( target->body.getW() * target->body.getH() ) / 10000.0f ;
//    
//    float movementMass = body.velX*weight;
//    
//    float otherMovementMass = (target->body.velX)*otherWeight;
//    
//    
//    body.velX += movementMass*otherMovementMass*(otherWeight/weight);
    
}

void CEntity::_doLogic() {
    if(!hasProperty(EntityProperty::FLIP_FREEZED)) {
        if(body.velX > 0)
            removeProperty(EntityProperty::FLIP);
        else if(body.velX < 0)
            addProperty(EntityProperty::FLIP);
    }
}

