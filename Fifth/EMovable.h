//
//  EMovable.h
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EMovable__
#define __Fifth__EMovable__

#include <stdio.h>

#include "EComponent.h"
#include "CEntity.h"


enum MovementState {
    WALKING_MOVEMENT = 0,
    SNEAKING_MOVEMENT,
    RUNNING_MOVEMENT
};

class EMovable : public EComponent {
    
public:
    EMovable(CEntity* parent);
    
    void onLoop(CInstance* instance);
    
    void goRight();
    void goLeft();
    void goUp();
    void goDown();
    bool hasWalkedX, hasWalkedY;
    
    void toggleNoclip();
    void jump();
    
    void setMovementState(MovementState movementState);
    
    float jumpPower;
    float accelerationX, accelerationY;
    float stoppingAccelerationX, stoppingAccelerationY;
    
private:
    MovementState _movementState;
    std::map<MovementState, int> _movementSpeeds;    // A map to get the speed at a specific movementState
    
};

#endif /* defined(__Fifth__EMovable__) */
