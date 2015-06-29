//
//  CServerInstance.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CServerInstance.h"

CServerInstance::CServerInstance() { }

CServerInstance::~CServerInstance() {
    closeInstance();
}

void CServerInstance::closeInstance() {
    entityManager.onCleanup();
}