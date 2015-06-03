//
//  CUtilityParticle.h
//  Fifth
//
//  Created by Didrik Munther on 01/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CUtilityParticle__
#define __Fifth__CUtilityParticle__

#include <stdio.h>
#include "CParticle.h"

class CUtilityParticle : public CParticle {
    
public:
    CUtilityParticle(sf::IntRect rect, sf::Color color);
    CUtilityParticle(sf::IntRect rect, sf::Color color, int livingTime);
    
private:
    virtual void _collisionLogic(CEntity* target);
    virtual void _doLogic();
    
    bool _doEffect;
    
    
};

#endif /* defined(__Fifth__CUtilityParticle__) */
