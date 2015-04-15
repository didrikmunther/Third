//
//  CPlayer.cpp
//  Third
//
//  Created by Didrik Munther on 14/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CPlayer.h"
#include "Define.h"

CPlayer::CPlayer(SDL_Rect rect, SDL_Color color) :
    CEntity(rect, color), maxSpeed(10.0f), accelerationX(1.0f), accelerationY(100.0f),
    stoppingAccelerationX(accelerationX * 4) {
}

void CPlayer::doLogic() {
    
    body.velY += GRAVITY;
}