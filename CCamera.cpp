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

CCamera::CCamera() :
    offset{0, 0, 0, 0}, cameraSway(10) {
}

void CCamera::onInit(CWindow* window) {
    offset = {0, 0, window->getWidth(), window->getHeight()};
}

void CCamera::onLoop() {
    if (target == nullptr) {
        offset.x = 0;
        offset.y = 0;
        return;
    }
    
    offset.x += (((target->body.getX() + target->body.getWidth() / 2) - offset.w / 2) - offset.x) / cameraSway;
    offset.y += (((target->body.getY() + target->body.getHeight() / 2) - offset.h / 2) - offset.y) / cameraSway;
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