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
#include "CCollidable.h"

class CGuiText;
class CCamera;
class CEntityManager;

class CEntity : public CRenderable, public CCollidable {
    
public:
    CEntity(Box rect, sf::Color color);
    CEntity(Box rect, std::string spriteContainerKey);
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
    
    bool collisionLeft, collisionRight;
    bool collisionTop, collisionBottom;
    
    sf::Color color;
    bool toRemove;
    
    void setSpriteContainer(std::string spriteContainerKey);
    CSpriteContainer* getSpriteContainer();
    std::string getSpriteContainerKey();
    std::string spriteContainerKey;
    bool hasSprite();
    
    bool isDead();
    
protected:
    std::vector<CGuiText*> _GuiTextVector;
    void _cleanUpTextVector();
    
    // Remember to allways call your parents _doLogic, and _collisionLogic function.
    virtual void _doLogic();
    virtual void _collisionLogic(CEntity* target);
    
    bool _isDead;
    
private:
    bool _collision(int x, int y, std::map<std::string, CEntity*>* entities);
    
};

#endif /* defined(__Third__CEntity__) */
