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
    CEntity(rect, color), color(color), creationTime(clock.getElapsedTime().asMilliseconds()), livingTime(5) {
}

CParticle::CParticle(sf::IntRect rect, sf::Color color, int livingTime) :
    CEntity(rect, color), color(color), creationTime(clock.getElapsedTime().asMilliseconds()), livingTime(livingTime) {
}

void CParticle::doLogic() {
    
    if(clock.getElapsedTime().asMilliseconds() - creationTime > livingTime * 1000)
        toRemove = true;
    
    body.velX += rand() % 3 - 1;
    //body.velY += rand() % 3 - 1;
    
    if(body.rect.top > DESPAWN_HEIGHT)
        toRemove = true;
}

