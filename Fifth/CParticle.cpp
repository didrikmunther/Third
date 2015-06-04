//
//  CParticle.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CParticle.h"
#include "Define.h"

CParticle::CParticle(sf::IntRect rect, sf::Color color) :
    CEntity(rect, color), _color(color), _creationTime(_clock.getElapsedTime().asMilliseconds()), _livingTime(5) {
}

CParticle::CParticle(sf::IntRect rect, sf::Color color, int livingTime) :
    CEntity(rect, color), _color(color), _creationTime(_clock.getElapsedTime().asMilliseconds()), _livingTime(livingTime) {
}

void CParticle::cParticleLoop() {
    if(_clock.getElapsedTime().asMilliseconds() - _creationTime > _livingTime * 1000)
        toRemove = true;
    
    if(body.rect.top > DESPAWN_HEIGHT)
        toRemove = true;
}

void CParticle::_doLogic() {
    CEntity::_doLogic();
    
    body.velX += rand() % 3 - 1;
}

