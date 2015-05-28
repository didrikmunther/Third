//
//  CBody.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CBody.h"

CBody::CBody(sf::IntRect rect) :
    velX(0), velY(0) {
    this->rect = rect;
}

int CBody::getX() {
    return rect.left;
}
int CBody::getY() {
    return rect.top;
}
int CBody::getW() {
    return rect.width;
}
int CBody::getH() {
    return rect.height;
}