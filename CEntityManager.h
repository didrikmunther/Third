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
#include <map>

class CCamera;

class CEntityManager {
    
public:
    CEntityManager();
    
    void addEntity(SDL_Rect rect, SDL_Color color);
    void addEntity(CEntity* entity);
    void addParticle(SDL_Rect rect, SDL_Color color, int livingTime);
    void addParticleEmitter(SDL_Rect rect, SDL_Color color, int amount, int frequency, int livingTime, float velocity);
    void onRender(SDL_Renderer *renderer, CCamera* camera);
    void onLoop();
    void onCleanup();
    void particleCleanup();
    
    int entityID;
    
private:
    //std::vector<std::shared_ptr<CEntity>> EntityVector;
    //std::vector<CEntity*> EntityVector;
    std::map<int, CEntity*> EntityVector;
    //std::vector<std::shared_ptr<CParticle>> ParticleVector;
    std::vector<CParticle*> ParticleVector;
    //std::vector<std::shared_ptr<CParticleEmitter>> ParticleEmitterVector;
    std::vector<CParticleEmitter*> ParticleEmitterVector;
    
};

#endif /* defined(__Third__CEntityManager__) */
