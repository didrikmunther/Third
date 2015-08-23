//
//  EComponent.h
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EComponent__
#define __Fifth__EComponent__

#include <stdio.h>
#include "NSurface.h"
#include "Define.h"
#include "CEntity.h"

class CInstance;
class CWindow;

class EComponent {
    
public:
    EComponent(CEntity* parent);
    
    virtual void onLoop(CInstance* instance) {  }
    virtual void onRender(CWindow* window, CCamera* camera) {  }
    virtual void renderAdditional(CWindow* window, CCamera* camera) {  };
    virtual bool collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides) { return true; }
    
    CEntity* _parent;
    
};

#endif /* defined(__Fifth__EComponent__) */
