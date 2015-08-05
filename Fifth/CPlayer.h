//
//  CPlayer.h
//  Third
//
//  Created by Didrik Munther on 14/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CPlayer__
#define __Third__CPlayer__

#include <stdio.h>
#include "CLiving.h"
#include "CSprite.h"
#include "CAssetManager.h"

class CPlayer : public CLiving {
    
public:
    CPlayer(Box rect, sf::Color color);
    CPlayer(Box rect, std::string spriteKey);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, int renderFlags);
    
protected:
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
private:
    void _init();
    
};

#endif /* defined(__Third__CPlayer__) */
