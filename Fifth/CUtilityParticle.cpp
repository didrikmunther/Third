//
//  CUtilityParticle.cpp
//  Fifth
//
//  Created by Didrik Munther on 01/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CUtilityParticle.h"
#include "CLiving.h"

#include <iostream>

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
    entityType = EntityTypes::UtilityParticle;
}

void CUtilityParticle::renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    CParticle::renderAdditional(window, camera, renderFlags);
    
}

bool CUtilityParticle::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CParticle::_collisionLogic(target, collisionSides);
    bool collision = true;
    
    if(target != _owner) {
        if(!toRemove()) {       // Avoid unneccesary dynamic_casts
            CLiving* living = dynamic_cast<CLiving*>(target);
            if(living != nullptr) {
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
            }
            _toRemove = true;
        }
    } else {
        collision = false;
    }
    
    return parentCollision && collision;
}

void CUtilityParticle::_doLogic() {
    CParticle::_doLogic();
    
    if(!collisionBottom)
        body.velY += rand() % 3 - 1;
}