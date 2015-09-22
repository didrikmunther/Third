//
//  EUtility.h
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EUtility__
#define __Fifth__EUtility__

#include <stdio.h>

#include "EComponent.h"


enum class BasicUtilities {
    DAMAGE = 0,
    HEAL,
    NONE
};

class EUtility : public EComponent {
    
public:
    EUtility(CEntity* parent, CEntity* owner, BasicUtilities basicUtility);
    
    bool onCollision(CEntity* target, CollisionSides* collisionSides);
    
private:
    BasicUtilities _basicUtility;
    CEntity* _owner;
    
    std::vector<CEntity*> _hasCollidedWith;
    
};

#endif /* defined(__Fifth__EUtility__) */
