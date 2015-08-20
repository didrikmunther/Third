//
//  EUtilityParticle.h
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EUtilityParticle__
#define __Fifth__EUtilityParticle__

#include <stdio.h>
#include "EParticle.h"

class EUtilityParticle : public EParticle {
    
public:
    EUtilityParticle(CEntity* parent, CEntity* owner, BasicUtilities utility, int livingTime = 5);
    
    void onLoop(CInstance* instance);
    void onRender(CWindow* window, CCamera* camera);
    void renderAdditional(CWindow* window, CCamera* camera);
    bool collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides);
    
protected:
    CEntity* _owner;
    BasicUtilities _utility;
    
};

#endif /* defined(__Fifth__EUtilityParticle__) */
