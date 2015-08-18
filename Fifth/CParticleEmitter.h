////
////  CParticleEmitter.h
////  Third
////
////  Created by Didrik Munther on 19/03/15.
////  Copyright (c) 2015 Didrik Munther. All rights reserved.
////
//
//#ifndef __Third__CParticleEmitter__
//#define __Third__CParticleEmitter__
//
//#include <stdio.h>
//#include "CParticle.h"
//
//enum ParticleTypes {
//    PARTICLE = 0,
//    UTILITY_PARTICLE
//};
//
//struct ParticleVelocity {
//    float x, y;
//};
//
//class CParticleEmitter {
//    
//public:
//    CParticleEmitter(SDL_Rect rect, SDL_Color color, int type, int amount, int frequency, int livingTime, int particleLivingTime, ParticleVelocity velocity);
//    CParticleEmitter(CParticle* particleTemplate, int amount, int frequency, int livingTime, ParticleVelocity velocity);
//    void onLoop(CEntityManager *entityManager);
//    
//    bool toRemove;
//    
//private:
//    
//    int type;
//    SDL_Rect _rect;
//    SDL_Color _color;
//    int _amount;
//    int _frequency;
//    int _creationTime;
//    int _livingTime;
//    int _particleLivingTime;
//    ParticleVelocity _velocity;
//    
//    int _timer;
//    
//};
//
//#endif /* defined(__Third__CParticleEmitter__) */
