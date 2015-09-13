//
//  CCollidable.h
//  Fifth
//
//  Created by Didrik Munther on 11/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CCollidable__
#define __Fifth__CCollidable__

#include <stdio.h>

#include "CBody.h"


enum EntityProperty {
    COLLIDABLE  = 1 << 0,
    FLYING      = 1 << 1,
    HIDDEN      = 1 << 2,
    STATIC      = 1 << 3,
    FLIP        = 1 << 4,
    FLIP_FREEZED= 1 << 5
};
    
class CCollidable {
    
public:
    CCollidable(Box rect);
    
    CBody body;
    
    int properties;
    bool hasProperty(int property);
    void toggleProperty(int property);
    void addProperty(int property);
    void removeProperty(int property);
    
private:
    
};

#endif /* defined(__Fifth__CCollidable__) */
