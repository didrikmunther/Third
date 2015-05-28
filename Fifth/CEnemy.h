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
    CEnemy(sf::IntRect rect, sf::Color color);
    CEnemy(sf::IntRect rect, std::string spriteKey);
    
    void setTarget(CEntity* target);
    CEntity* getTarget();
    
private:
    virtual void _initMovementValues();
    virtual void _doLogic();
    
    CEntity* target;
    
};

#endif /* defined(__Fifth__CEnemy__) */
