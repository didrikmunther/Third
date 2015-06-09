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

enum BasicUtilities {
    DAMAGE = 0,
    HEAL,
    NONE
};

class CUtilityParticle : public CParticle {
    
public:
    CUtilityParticle(sf::IntRect rect, sf::Color color, int utility);
    CUtilityParticle(sf::IntRect rect, sf::Color color, int utility, int livingTime);
    
protected:
    virtual void _doLogic();
    virtual void _collisionLogic(CEntity* target);
    
private:
    CEntity* _owner;
    
    int _basicUtility;
    
};

#endif /* defined(__Fifth__CUtilityParticle__) */
