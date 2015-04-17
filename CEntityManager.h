//
//  CEntityManager.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntityManager__
#define __Third__CEntityManager__

#include <SDL2/SDL.h>
#include <stdio.h>
#include <memory>
#include <vector>
#include "CEntity.h"
#include "CParticle.h"
#include "CParticleEmitter.h"
#include "CSprite.h"
#include <map>
#include <string>

class CCamera;

class CEntityManager {
    
public:
    CEntityManager();
    
    CEntity* addEntity(SDL_Rect rect, SDL_Color color);
    CEntity* addEntity(SDL_Rect rect, CSprite* sprite);
    void addEntity(CEntity* entity);
    void addParticle(SDL_Rect rect, SDL_Color color, int livingTime);
    void addParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, int particleLivingTime, float velocity);
    void onRender(SDL_Renderer *renderer, CCamera* camera);
    void onLoop();
    void onCleanup();
    
    void entityCleanup();
    void particleEmitterCleanup();
    void particleCleanup();
    
    int entityID;
    
private:
    std::map<std::string, CEntity*> EntityVector;
    std::vector<CParticle*> ParticleVector;
    std::vector<CParticleEmitter*> ParticleEmitterVector;
    
};

#endif /* defined(__Third__CEntityManager__) */
