//
//  CMovable.h
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CMovable__
#define __Fifth__CMovable__

#include <stdio.h>
#include "CEntity.h"
#include <SFML/Graphics.hpp>

class CMovable : public CEntity {
    
public:
    CMovable(Box rect, sf::Color color);
    CMovable(Box rect, std::string spriteKey);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, int renderFlags);
    
    void goRight();
    void goLeft();
    void goUp();
    void goDown();
    bool hasWalkedX, hasWalkedY;
    
    void toggleNoclip();
    void jump();
    bool isSneaking;
    
    float maxSpeed;
    float jumpPower;
    float accelerationX, accelerationY;
    float stoppingAccelerationX;
    float sneakSpeed;
    
protected:
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    virtual void _doLogic();
    
private:
    void _init();
    
};

#endif /* defined(__Fifth__CMovable__) */
