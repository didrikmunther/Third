//
//  EEnemy.h
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EEnemy__
#define __Fifth__EEnemy__

#include <stdio.h>

#include "EComponent.h"
#include "NSurface.h"


class EEnemy : public EComponent {
    
public:
    EEnemy(CEntity* parent);
    
    void onLoop(CInstance* instance);
    void onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    bool onCollision(CEntity* target, CollisionSides* collisionSides);
    
    void setTarget(CEntity* target);
    CEntity* getTarget();
    
    void shootTarget();
    
private:
    std::vector<Triangle> _triangles;
    
    CEntity* _target;
    
    int _shootTimer;
    bool _renderTriangle;
    
};

#endif /* defined(__Fifth__EEnemy__) */
