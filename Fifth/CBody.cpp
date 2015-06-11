//
//  CBody.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CBody.h"

CBody::CBody(Box rect) :
    velX(0), velY(0) {
    this->rect = rect;
}

int CBody::getX() {
    return rect.x;
}
int CBody::getY() {
    return rect.y;
}
int CBody::getW() {
    return rect.w;
}
int CBody::getH() {
    return rect.h;
}