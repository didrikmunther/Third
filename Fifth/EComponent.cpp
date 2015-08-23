//
//  EComponent.cpp
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EComponent.h"
#include <iostream>

EComponent::EComponent(CEntity* parent) {
    if(parent)
        _parent = parent;
    else {
        delete this;
        std::cout << "errror\n";
    }
}