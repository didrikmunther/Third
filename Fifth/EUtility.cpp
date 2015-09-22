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

#include <iostream>


EUtility::EUtility(CEntity* parent, CEntity* owner, BasicUtilities basicUtility) : EComponent(parent) {
    parent->collisionLayer = LAYER0;
    _basicUtility = basicUtility;
    _owner = owner;
}

bool EUtility::onCollision(CEntity* target, CollisionSides* collisionSides) {
    if(target == _owner && _basicUtility != BasicUtilities::HEAL)
        return false;
    
    if(*toRemove())
        return true;
    
    if(std::find(_hasCollidedWith.begin(), _hasCollidedWith.end(), target) != _hasCollidedWith.end())
        return false;
    
    ELiving* living = target->getComponent<ELiving>();
    if(!living) {
        *toRemove() = true;
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