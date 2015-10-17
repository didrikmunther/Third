//
//  CBody.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CBody.h"


CBody::CBody(Box rect)
    : velX(0), velY(0)
    , _rect(rect), _previousRect(rect)
{
    
}

void CBody::onLoop() {
    _previousRect = _rect;
}

int CBody::getX() {
    return _rect.x;
}
int CBody::getY() {
    return _rect.y;
}
int CBody::getW() {
    return _rect.w;
}
int CBody::getH() {
    return _rect.h;
}