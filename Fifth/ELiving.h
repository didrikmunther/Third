//
//  ELiving.h
//  Fifth
//
//  Created by Didrik Munther on 20/08/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__ELiving__
#define __Fifth__ELiving__

#include <stdio.h>
#include "EComponent.h"

class ELiving : public EComponent {
    
public:
    ELiving(CEntity* parent);
    
    void onLoop(CInstance* instance);
    void onRender(CWindow* window, CCamera* camera);
    void renderAdditional(CWindow* window, CCamera* camera);
    bool collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides);
    
    int dealDamage(int amount, UtilityPosition position = {0, 0}, CEntity* damager = nullptr);
    int heal(int amount, UtilityPosition position = {0, 0}, CEntity* healer = nullptr);
    
private:
    int _values[ValueTypes::VALUETYPES_TOTAL];
    int _maxValues[ValueTypes::VALUETYPES_TOTAL];
    int _stats[StatTypes::STATTYPES_TOTAL];
    
};

#endif /* defined(__Fifth__ELiving__) */
