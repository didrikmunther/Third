//
//  CParticleEmitter.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CParticleEmitter__
#define __Third__CParticleEmitter__

enum ParticleTypes {
    PARTICLE = 0,
    UTILITY_PARTICLE
};

#include <stdio.h>
#include "CParticle.h"
#include <SFML/Graphics.hpp>

class CParticleEmitter {
    
public:
    CParticleEmitter(sf::IntRect rect, sf::Color color, int type, int amount, int frequency, int livingTime, int particleLivingTime, float velocity);
    void onLoop(CEntityManager *entityManager);
    
    bool toRemove;
    
private:
    
    sf::Clock _clock;
    
    int type;
    sf::IntRect _rect;
    sf::Color _color;
    int _amount;
    int _frequency;
    int _creationTime;
    int _livingTime;
    int _particleLivingTime;
    float _velocity;
    
    int _timer;
    
};

#endif /* defined(__Third__CParticleEmitter__) */
