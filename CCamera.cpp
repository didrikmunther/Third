//
//  CCamera.cpp
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CCamera.h"

CCamera::CCamera(CEntity* target, int WIDTH, int HEIGHT) :
    target(target), offset{0, 0, WIDTH, HEIGHT} {
}
CCamera::CCamera(int WIDTH, int HEIGHT) :
    offset{0, 0, WIDTH, HEIGHT} {
}

int CCamera::offsetX() {
    if(target == nullptr) {
        return 0;
    }
    oldOffset.x = offset.x;
    offset.x = (target->body.getX() + target->body.getWidth() / 2) - offset.w / 2;
    return offset.x;
}

int CCamera::offsetY() {
    if(target == nullptr) {
        return 0;
    }
    oldOffset.y = offset.y;
    offset.y = (target->body.getY() + target->body.getHeight() / 2) - offset.h / 2;
    return offset.y;
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