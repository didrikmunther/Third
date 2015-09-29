//
//  EComponent.cpp
//  Fifth
//
//  Created by Didrik Munther on 21/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EComponent.h"
#include "CEntity.h"


EComponent::EComponent(CEntity* parent)
    : parent(parent) {
}

EComponent::~EComponent() {
    
}

std::vector<CGuiText*>* EComponent::guiTextVector() {
    return &parent->_GuiTextVector;
}