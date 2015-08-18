//
//  CNpc.h
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CNpc__
#define __Fifth__CNpc__

#include <stdio.h>
#include "CLiving.h"

class CNpc : public CLiving {
    
public:
    CNpc(Box rect, SDL_Color color);
    CNpc(Box rect, std::string spriteKey);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
protected:
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
private:
    void _init();
};

#endif /* defined(__Fifth__CNpc__) */
