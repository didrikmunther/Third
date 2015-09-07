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
    CUtilityParticle(Box rect, SDL_Color color, CEntity* owner, BasicUtilities utility);
    CUtilityParticle(Box rect, SDL_Color color, CEntity* owner, BasicUtilities utility, int livingTime);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
protected:
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
private:
    void _init();
    
    CEntity* _owner;
    
    BasicUtilities _basicUtility;
    
    std::vector<CEntity*> _hasCollidedWith;
    
};

#endif /* defined(__Fifth__CUtilityParticle__) */
