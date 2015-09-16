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
#include "NSurface.h"


class CEnemy : public CNpc {
    
public:
    CEnemy(Box rect, SDL_Color color);
    CEnemy(Box rect, std::string spriteKey);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
    void setTarget(CEntity* target);
    CEntity* getTarget();
    
private:
    void _init();
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
    std::vector<Triangle> triangles;
    
    CEntity* target;
    
    int _shootTimer;
    
};

#endif /* defined(__Fifth__CEnemy__) */
