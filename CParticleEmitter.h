//
//  CParticleEmitter.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CParticleEmitter__
#define __Third__CParticleEmitter__

#include <stdio.h>
#include "CParticle.h"
//#include "CEntityManager.h"

class CEntityManager;

class CParticleEmitter {
    
public:
    CParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, float velocity);
    void onLoop(CEntityManager *entityManager);
    
    bool toRemove;
    
private:
    SDL_Rect rect;
    SDL_Color color;
    int amount;
    int frequency;
    int creationTime;
    int livingTime;
    float velocity;
    
    int timer;
    
};

#endif /* defined(__Third__CParticleEmitter__) */
