//
//  CEntity.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntity__
#define __Third__CEntity__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include "CBody.h"
#include "CSprite.h"
#include "CAssetManager.h"

class CCamera;
class CEntityManager;

class CEntity {
    
public:
    CEntity(sf::IntRect rect, sf::Color color);
    //CEntity(SDL_Rect rect, CSprite* sprite);
    CEntity(sf::IntRect rect, std::string spriteKey, CAssetManager* assetManager);
    void initValues();
    void onLoop(std::map<std::string, CEntity*>* entities);
    virtual void doLogic();
    void move(std::map<std::string, CEntity*>* entities);
    bool collision(int x, int y, std::map<std::string, CEntity*>* entities);
    void onRender(sf::RenderWindow* window, CCamera* camera, int renderFlags);
    int properties;
    void say(std::string text, std::string fontKey, CAssetManager* assetManager, CEntityManager* entityManager, int type);
    
    bool hasProperty(int property);
    void toggleProperty(int property);
    void addProperty(int property);
    void removeProperty(int property);
    
    bool collisionLeft, collisionRight;
    bool collisionTop, collisionBottom;
    
    CBody body;
    bool toRemove;
    
    //int setSprite(CSprite* sprite);
    int setSprite(std::string spriteKey);
    //CSprite* sprite;
    std::string spriteKey;
    CAssetManager* assetManager;
    
    bool flip;
    
protected:
    sf::Color color;
    
};

#endif /* defined(__Third__CEntity__) */
