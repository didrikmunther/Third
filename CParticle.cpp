//
//  CParticle.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CParticle.h"

CParticle::CParticle(SDL_Rect rect, SDL_Color color) :
    CEntity(rect, color), color(color), creationTime(SDL_GetTicks()), livingTime(5) {
}

CParticle::CParticle(SDL_Rect rect, SDL_Color color, int livingTime) :
    CEntity(rect, color), color(color), creationTime(SDL_GetTicks()), livingTime(livingTime) {
}

void CParticle::doLogic() {
    
    if(SDL_GetTicks() - creationTime > livingTime * 1000)
        toRemove = true;
    
    body.velX += rand() % 3 - 1;
    body.velY += 0.2;
    //body.velY += rand() % 3 - 1;
}

