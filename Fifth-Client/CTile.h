//
//  CTile.h
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CTile__
#define __Third__CTile__

#include <stdio.h>
#include <SFML/Graphics.hpp>

class CTile {
    
public:
    CTile(sf::IntRect rect, int type);
    
    void onRender(sf::RenderTarget& renderTarget);
    
private:
    int _type;
    sf::IntRect _rect;
    
};

#endif /* defined(__Third__CTile__) */
