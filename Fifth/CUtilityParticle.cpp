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

CUtilityParticle::CUtilityParticle(sf::IntRect rect, sf::Color color) : CParticle(rect, color) {
    
}

CUtilityParticle::CUtilityParticle(sf::IntRect rect, sf::Color color, int livingTime) : CParticle(rect, color, livingTime) {
    
}

void CUtilityParticle::_collisionLogic(CEntity* target) {
    CLiving* living = dynamic_cast<CLiving*>(target);
    if(living != nullptr && !toRemove) {
        living->dealDamage(10, DamagePosition{body.getX(), body.getY(), true});
        toRemove = true;
    }
}

void CUtilityParticle::_doLogic() {
    CParticle::_doLogic();
    
    body.velY += rand() % 3 - 1;
}