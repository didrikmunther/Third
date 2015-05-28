//
//  CNpc.h
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CNpc__
#define __Fifth__CNpc__

#include <stdio.h>
#include "CMovable.h"
#include <SFML/Graphics.hpp>

class CNpc : public CMovable {
    
public:
    CNpc(sf::IntRect rect, sf::Color color);
    CNpc(sf::IntRect rect, std::string spriteKey);
    
private:
    virtual void _initMovementValues();
    virtual void _doLogic();
};

#endif /* defined(__Fifth__CNpc__) */
