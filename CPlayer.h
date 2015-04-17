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
#include "CEntity.h"
#include "CSprite.h"

class CPlayer : public CEntity {
    
public:
    CPlayer(SDL_Rect rect, SDL_Color color);
    CPlayer(SDL_Rect rect, CSprite* sprite);
    
    void jump();
    
    float maxSpeed;
    float accelerationX, accelerationY;
    float stoppingAccelerationX;
    
    
private:
    virtual void doLogic();
    
    
};

#endif /* defined(__Third__CPlayer__) */
