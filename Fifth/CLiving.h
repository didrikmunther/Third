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

struct DamagePosition {
    int x, y;
    bool hasPosition;
};

class CLiving : public CMovable {
    
public:
    CLiving(sf::IntRect rect, sf::Color color);
    CLiving(sf::IntRect rect, std::string spriteKey);
    
    void dealDamage(int amount, DamagePosition position = {0, 0, false});
    void heal(int amount);

    void cLivingRender(CWindow *window, CCamera *camera);
    
protected:
    virtual void _doLogic();
    virtual void _collisionLogic(CEntity* target);
    
private:
    
    void _initValues();
    
    int _values[ValueTypes::VALUETYPES_TOTAL];
    int _maxValues[ValueTypes::VALUETYPES_TOTAL];
    int _stats[StatTypes::STATTYPES_TOTAL];
    
};

#endif /* defined(__Fifth__CLiving__) */
