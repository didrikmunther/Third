//
//  CAnimation.h
//  Fifth
//
//  Created by Didrik Munther on 08/07/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CAnimation__
#define __Fifth__CAnimation__

#include <stdio.h>
#include <vector>

#include "CSprite.h"


class CAnimation : public CSprite {

public:
    CAnimation(int imagesPerSecond);
    
private:
    float _imagesPerSecond;
    
};

#endif /* defined(__Fifth__CAnimation__) */
