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

CUtilityParticle::CUtilityParticle(sf::IntRect rect, sf::Color color, int utility) :
CParticle(rect, color), _basicUtility(utility) {
    std::cout << utility << "\n";
}

CUtilityParticle::CUtilityParticle(sf::IntRect rect, sf::Color color, int utility, int livingTime) :
CParticle(rect, color, livingTime), _basicUtility(utility) {
    
}

void CUtilityParticle::_collisionLogic(CEntity* target) {
    CParticle::_collisionLogic();
    
    CLiving* living = dynamic_cast<CLiving*>(target);
    if(living != nullptr && !toRemove) {
        switch(_basicUtility) {
            case BasicUtilities::DAMAGE:
                living->dealDamage(rand() % 20, UtilityPosition{body.getX(), body.getY()});
                toRemove = true;
                break;
                
            case BasicUtilities::HEAL:
                living->heal(rand() % 20, UtilityPosition{body.getX(), body.getY()});
                toRemove = true;
                break;
        }
    }
}

void CUtilityParticle::_doLogic() {
    CParticle::_doLogic();
    
    body.velY += rand() % 3 - 1;
}