//
//  CSpriteSheet.h
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CSpriteSheet__
#define __Third__CSpriteSheet__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string>

class CSpriteSheet {
    
public:
    CSpriteSheet(std::string fileName);
    void openFile(std::string fileName);
    void onCleanup();
    
    sf::Texture* getTexture();
    
private:
    
    sf::Texture texture;
    
};

#endif /* defined(__Third__CSpriteSheet__) */
