//
//  CNpc.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CNpc.h"

CNpc::CNpc(sf::IntRect rect, sf::Color color) :
CMovable(rect, color) {
    
}

CNpc::CNpc(sf::IntRect rect, std::string spriteKey) :
CMovable(rect, spriteKey) {
    
}

void CNpc::_doLogic() {
    goRight();
    checkMovement();
}