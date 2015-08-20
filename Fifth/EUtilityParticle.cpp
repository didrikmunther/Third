//
//  EUtilityParticle.cpp
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EUtilityParticle.h"
#include "ELiving.h"

EUtilityParticle::EUtilityParticle(CEntity* parent, CEntity* owner, BasicUtilities utility, int livingTime /* = 5 */) : EParticle(parent, livingTime), _owner(owner), _utility(utility) {
    
}

void EUtilityParticle::onLoop(CInstance* instance) {
    EParticle::onLoop(instance);
    
    if(!_parent->collisionBottom)
        _parent->body.velY += rand() % 3 - 1;
}

void EUtilityParticle::onRender(CWindow* window, CCamera* camera) {
    EParticle::onRender(window, camera);
    
}

void EUtilityParticle::renderAdditional(CWindow* window, CCamera* camera) {
    
}

bool EUtilityParticle::collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides) {
    bool toReturn = EParticle::collisionLogic(target, instance, collisionSides);
    
    if(target != _owner) {
        if(!_parent->toRemove) {       // Avoid unneccesary dynamic_casts
            ELiving* living = target->getComponent<ELiving>();
            if(living) {
                switch(_utility) {
                    case BasicUtilities::DAMAGE:
                        living->dealDamage(rand() % 20, UtilityPosition{_parent->body.getX(), _parent->body.getY()});
                        break;
                        
                    case BasicUtilities::HEAL:
                        living->heal(rand() % 20, UtilityPosition{_parent->body.getX(), _parent->body.getY()});
                        break;
                        
                    default:
                        break;
                }
            }
            _parent->toRemove = true;
        }
    } else {
        return false && toReturn;
    }
    
    return true && toReturn;
}
