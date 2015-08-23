//
//  EParticle.h
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EParticle__
#define __Fifth__EParticle__

#include <stdio.h>
#include "EComponent.h"

class EParticle : public EComponent {
    
public:
    EParticle(CEntity* parent, int livingTime);
    
    virtual void onLoop(CInstance* instance);
    virtual void onRender(CWindow* window, CCamera* camera);
    virtual void renderAdditional(CWindow* window, CCamera* camera);
    virtual bool collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides);
    
protected:
    int _livingTime;
    int _creationTime;
    
};

#endif /* defined(__Fifth__EParticle__) */
