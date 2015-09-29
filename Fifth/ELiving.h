//
//  ELiving.h
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__ELiving__
#define __Fifth__ELiving__

#include <stdio.h>

#include "EComponent.h"


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

class ELiving : public EComponent {
    
public:
    ELiving(CEntity* parent);
    
    void onLoop(CInstance* instance);
    void onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    bool onCollision(CEntity* target, CollisionSides* collisionSides);
    
    void serialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc);
    void deserialize(rapidjson::Value* value);
    
    int dealDamage(int amount, UtilityPosition position = {0, 0}, CEntity* damager = nullptr);
    int heal(int amount, UtilityPosition position = {0, 0}, CEntity* healer = nullptr);
    
    bool _hasTakenFallDamage;
    
protected:
    int _values[ValueTypes::VALUETYPES_TOTAL];
    int _maxValues[ValueTypes::VALUETYPES_TOTAL];
    
    int _bufferedValues[ValueTypes::VALUETYPES_TOTAL];
    int _animatedBufferedValues[ValueTypes::VALUETYPES_TOTAL];
    float _animatedBufferedIncrements[ValueTypes::VALUETYPES_TOTAL];
    
    int _stats[StatTypes::STATTYPES_TOTAL];
    
private:
    int _delay;
    int _timer;
    
    bool _hasBeenDamaged;
    bool _hasBeenHealed;
    
};


#endif /* defined(__Fifth__ELiving__) */
