//
//  CUtilityParticle.cpp
//  Fifth
//
//  Created by Didrik Munther on 01/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CUtilityParticle.h"
#include "CLiving.h"


CUtilityParticle::CUtilityParticle(Box rect, SDL_Color color, CEntity* owner, BasicUtilities utility) :
CParticle(rect, color), _owner(owner), _basicUtility(utility) {
    _init();
}

CUtilityParticle::CUtilityParticle(Box rect, SDL_Color color, CEntity* owner, BasicUtilities utility, int livingTime) :
CParticle(rect, color, livingTime), _owner(owner), _basicUtility(utility) {
    _init();
}

void CUtilityParticle::_init() {
    collisionLayer = LAYER0;
}

void CUtilityParticle::renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    CParticle::renderAdditional(window, camera, renderFlags);
    
}

bool CUtilityParticle::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CParticle::_collisionLogic(target, collisionSides);
    
    if(target == _owner && _basicUtility != BasicUtilities::HEAL)
        return false;
    
    if(toRemove())
        return parentCollision;
    
    if(std::find(_hasCollidedWith.begin(), _hasCollidedWith.end(), target) != _hasCollidedWith.end())
        return false;
    
//    if(_hasCollidedWith.size() > 2) {
//        _toRemove = true;
//        return parentCollision;
//    }
    
    CLiving* living = dynamic_cast<CLiving*>(target);
    if(living == nullptr) {
        _toRemove = true;
        return parentCollision;
    }
    
    switch(_basicUtility) {
        case BasicUtilities::DAMAGE:
            living->dealDamage(rand() % 20, UtilityPosition{body.getX(), body.getY()});
            break;
        case BasicUtilities::HEAL:
            living->heal(rand() % 20, UtilityPosition{body.getX(), body.getY()});
            break;
            
        default:
            break;
    }
    
    _hasCollidedWith.push_back(target);
    
    return false;
}

void CUtilityParticle::_doLogic() {
    CParticle::_doLogic();
    
//    if(!collisionSides.bottom)
//        body.velY += rand() % 3 - 1;
}