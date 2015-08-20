//
//  ENpc.h
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__ENpc__
#define __Fifth__ENpc__

#include <stdio.h>
#include "EComponent.h"

class ENpc : public EComponent {
    
public:
    ENpc(CEntity* parent);
    
    void onLoop(CInstance* instance);
    void onRender(CWindow* window, CCamera* camera);
    void renderAdditional(CWindow* window, CCamera* camera);
    bool collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides);
    
    void setTarget(CEntity* target);
    CEntity* getTarget();
    
protected:
    CEntity* _target;
    
};

#endif /* defined(__Fifth__ENpc__) */
