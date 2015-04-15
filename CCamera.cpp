//
//  CCamera.cpp
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CCamera.h"
#include <iostream>
#include "Define.h"

CCamera::CCamera(CEntity* target, int WIDTH, int HEIGHT) :
    target(target), offset{0, 0, WIDTH, HEIGHT} {
}
CCamera::CCamera(int WIDTH, int HEIGHT) :
    offset{0, 0, WIDTH, HEIGHT} {
}

void CCamera::onLoop() {
    if (target == nullptr) {
        offset.x = 0;
        offset.y = 0;
    }
    
    //offset.x = (target->body.getX() + target->body.getWidth() / 2) - offset.w / 2;
    offset.x += (((target->body.getX() + target->body.getWidth() / 2) - offset.w / 2) - offset.x) / CAMERA_SWAY;
    
    //offset.y = (target->body.getY() + target->body.getHeight() / 2) - offset.h / 2;
    offset.y += (((target->body.getY() + target->body.getHeight() / 2) - offset.h / 2) - offset.y) / CAMERA_SWAY;
}

int CCamera::offsetX() {
    return (int)floor(offset.x);
}

int CCamera::offsetY() {
    return (int)floor(offset.y);
}

void CCamera::setTarget(CEntity* target) {
    this->target = target;
}

bool CCamera::collision(CEntity* entity) {
    if(offsetX() > entity->body.getX() + entity->body.getWidth())
        return false;
    if(offsetX() + offset.w < entity->body.getX())
        return false;
    if(offsetY() > entity->body.getY() + entity->body.getHeight())
        return false;
    if(offsetY() + offset.h < entity->body.getY())
        return false;
    else
        return true;
}