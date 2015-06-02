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
#include "CRenderable.h"
#include "CWindow.h"

enum EntityProperty {
    COLLIDABLE  = 1 << 0,
    FLYING      = 1 << 1,
    HIDDEN      = 1 << 2,
    STATIC      = 1 << 3,
    FLIP        = 1 << 4
};

class CGuiText;
class CCamera;
class CEntityManager;

class CEntity : public CRenderable {
    
public:
    CEntity(sf::IntRect rect, sf::Color color);
    CEntity(sf::IntRect rect, std::string spriteKey);
    ~CEntity();
    
    void initValues();
    void onLoop(std::map<std::string, CEntity*>* entities);
    void onRender(CWindow* window, CCamera* camera, int renderFlags);
    
    int collisionLayer;
    bool isOnCollisionLayer(int collisionLayer);
    
    void move(std::map<std::string, CEntity*>* entities);
    bool coordinateCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);
    bool coordinateCollision(int x, int y, int w, int h);
    
    void say(std::string text, std::string fontKey, int type);
    void renderAdditional(CWindow* window, CCamera* camera, int renderFlags);
    
    int properties;
    bool hasProperty(int property);
    void toggleProperty(int property);
    void addProperty(int property);
    void removeProperty(int property);
    
    bool collisionLeft, collisionRight;
    bool collisionTop, collisionBottom;
    
    sf::Color color;
    CBody body;
    bool toRemove;
    
    void setSprite(std::string spriteKey);
    CSprite* getSprite();
    std::string spriteKey;
    
protected:
    std::vector<CGuiText*> _GuiTextVector;
    
private:
    virtual void _collisionLogic(CEntity* target);
    virtual void _doLogic();
    
    bool _collision(int x, int y, std::map<std::string, CEntity*>* entities);
    
};

#endif /* defined(__Third__CEntity__) */
