//
//  CParticle.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CParticle__
#define __Third__CParticle__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "CEntity.h"

class CParticle : public CEntity {
    
public:
    CParticle(Box rect, sf::Color color, int livingTime = 5);
    CParticle(Box rect, std::string spriteKey, int livingTime = 5);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
protected:
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
    int _livingTime;
    
private:
    
    void _init();
    
    sf::Clock _clock;
    int _creationTime;
};


#endif /* defined(__Third__CParticle__) */
