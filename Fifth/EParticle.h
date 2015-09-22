//
//  EParticle.h
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__EParticle__
#define __Fifth__EParticle__

#include <stdio.h>

#include "EComponent.h"


class EParticle : public EComponent {
    
public:
    EParticle(CEntity* parent, int livingTime);
    
    void onLoop(CInstance* instance);
    
private:
    int _creationTime;
    int _livingTime;
    
};

#endif /* defined(__Fifth__EParticle__) */
