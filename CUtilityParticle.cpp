//
//  CUtilityParticle.cpp
//  Fifth
//
//  Created by Didrik Munther on 01/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CUtilityParticle.h"
#include "CLiving.h"

CUtilityParticle::CUtilityParticle(sf::IntRect rect, sf::Color color) : CParticle(rect, color) {
    
}

CUtilityParticle::CUtilityParticle(sf::IntRect rect, sf::Color color, int livingTime) : CParticle(rect, color, livingTime) {
    
}

void CUtilityParticle::_collisionLogic(CEntity* target) {
    CLiving* living = dynamic_cast<CLiving*>(target);   // If a living entity, call the function for CLiving
    if(living != nullptr) {
        living->dealDamage(1);
        toRemove = true;
    }
}