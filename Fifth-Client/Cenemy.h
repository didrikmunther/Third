//
//  Cenemy.h
//  Fifth-Client
//
//  Created by Lucas Karlsson on 27/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth_Client__Cenemy__
#define __Fifth_Client__Cenemy__

#include <stdio.h>
#include "CMovable.h"
#include <SFML/Graphics.hpp>

class Cenemy : public CMovable {
    

public:
    Cenemy(sf::IntRect rect, sf::Color color);
    Cenemy(sf::IntRect rect, std::string spriteKey);
    
    void initValues();
    
    
    
private:
    virtual void _doLogic();
    
    
};


#endif /* defined(__Fifth_Client__Cenemy__) */
