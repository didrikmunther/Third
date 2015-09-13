//
//  CInstance.cpp
//  Fifth
//
//  Created by Didrik Munther on 06/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CInstance.h"


CInstance::CInstance() {
}

void CInstance::closeInstance() {
    entityManager.onCleanup();
}