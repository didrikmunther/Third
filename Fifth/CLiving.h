//
//  CLiving.h
//  Fifth
//
//  Created by Didrik Munther on 01/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CLiving__
#define __Fifth__CLiving__

#include <stdio.h>
#include "CWindow.h"
#include "CCamera.h"
#include "CMovable.h"

enum ValueTypes {
    HEALTH = 0,
    KEVLAR,
    STAMINA,
    ENERGY,
    VALUETYPES_TOTAL
};

enum StatTypes {
    ARMOUR = 0,
    ATTACK_POWER,
    ATTACK_SPEED,
    STATTYPES_TOTAL
};

struct UtilityPosition {
    int x, y;
};

class CLiving : public CMovable {
    
public:
    CLiving(Box rect, SDL_Color color);
    CLiving(Box rect, std::string spriteKey);
    
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
    int dealDamage(int amount, UtilityPosition position = {0, 0});
    int heal(int amount, UtilityPosition position = {0, 0});

    void cLivingRender(CWindow *window, CCamera *camera);
    
protected:
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
    int _values[ValueTypes::VALUETYPES_TOTAL];
    int _maxValues[ValueTypes::VALUETYPES_TOTAL];
    int _stats[StatTypes::STATTYPES_TOTAL];
    
private:
    
    void _init();
    
};

#endif /* defined(__Fifth__CLiving__) */
