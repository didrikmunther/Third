//
//  CEnemy.h
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CEnemy__
#define __Fifth__CEnemy__

#include <stdio.h>
#include "CNpc.h"
#include <SFML/Graphics.hpp>
#include "CEntity.h"

class CEnemy : public CNpc {
    
public:
    CEnemy(Box rect, sf::Color color);
    CEnemy(Box rect, std::string spriteKey);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, int renderFlags);
    
    void setTarget(CEntity* target);
    CEntity* getTarget();
    
private:
    void _init();
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
    CEntity* target;
    
};

#endif /* defined(__Fifth__CEnemy__) */
