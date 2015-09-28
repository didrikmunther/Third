//
//  EUtility.cpp
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EUtility.h"
#include "ELiving.h"

#include "CEntity.h"
#include "NSurface.h"
#include "CWindow.h"

#include <iostream>


EUtility::EUtility(CEntity* parent, CEntity* owner, BasicUtilities basicUtility) : EComponent(parent) {
    parent->collisionLayer = LAYER0;
    _basicUtility = basicUtility;
    _owner = owner;
    removeTimer = -1;
}

EUtility::~EUtility() {
    positions.clear();
}

void EUtility::onLoop(CInstance* instance) {
    positions.push_back(Position{parent->body.getX(), parent->body.getY()});
    
    if(removeTimer != -1)
        removeTimer--;
    
    if(removeTimer == 0)
        *toRemove() = true;
}

void EUtility::onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    
    if(renderFlags & RenderFlags::TRACE_BULLETS && positions.size() >= 2) {
        Color damageColor = {255, 0, 0, 255};
        Color healingColor = {0, 255, 0, 255};
        Color color = _basicUtility == BasicUtilities::DAMAGE ? damageColor : healingColor ;
        
        Position oldPos = positions[0];
        
        for(auto &i: positions) {
            NSurface::renderLine(Line{oldPos, i, color}, window->getRenderer(), camera);
            oldPos = i;
        }
    }
}

bool EUtility::onCollision(CEntity* target, CollisionSides* collisionSides) {
    if(removeTimer != -1)
        return true;
    
    if(target == _owner && _basicUtility != BasicUtilities::HEAL)
        return false;
    
    if(*toRemove())
        return true;
    
    if(std::find(_hasCollidedWith.begin(), _hasCollidedWith.end(), target) != _hasCollidedWith.end())
        return false;
    
    ELiving* living = target->getComponent<ELiving>();
    if(!living) {
        removeTimer = 200;
        parent->addProperty(EntityProperty::STATIC);
        parent->addProperty(EntityProperty::HIDDEN);
        return true;
    }
    
    switch(_basicUtility) {
        case BasicUtilities::DAMAGE:
            living->dealDamage(rand() % 20, UtilityPosition{parent->body.getX(), parent->body.getY()});
            break;
        case BasicUtilities::HEAL:
            living->heal(rand() % 20, UtilityPosition{parent->body.getX(), parent->body.getY()});
            break;
            
        default:
            break;
    }
    
    _hasCollidedWith.push_back(target);
    
    return false;
}