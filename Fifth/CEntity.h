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
#include "CChatBubble.h"

struct CollisionSides {
    bool collisionTop,
         collisionBottom,
         collisionRight,
         collisionLeft;
};

class CGuiText;
class CCamera;
class CEntityManager;

class CEntity : public CRenderable, public CCollidable {
    
public:
    CEntity(Box rect, sf::Color color);
    CEntity(Box rect, std::string spriteContainerKey);
    ~CEntity();
    
    EntityTypes entityType;
    
    virtual void init();
    void onLoop(std::map<std::string, CEntity*>* entities);
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
    int collisionLayer;
    bool isOnCollisionLayer(int collisionLayer);
    void toggleCollisionLayer(int collisionLayer) { this->collisionLayer ^= collisionLayer; }
    void addCollisionLayer(int collisionLayer) { this->collisionLayer |= collisionLayer; }
    void removeCollisionLayer(int collisionLayer) { if(isOnCollisionLayer(collisionLayer)) toggleCollisionLayer(collisionLayer); }
    
    void move(std::map<std::string, CEntity*>* entities);
    bool coordinateCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);
    bool coordinateCollision(int x, int y, int w, int h);
    
    void say(std::string text, std::string fontKey, ChatBubbleType type);
    
    bool collisionLeft, collisionRight;
    bool collisionTop, collisionBottom;
    
    std::string spriteStateTypes[SpriteStateTypes::TOTAL_SPRITESTATETYPES];
    
    void setSpriteContainer(std::string spriteContainerKey);
    CSpriteContainer* getSpriteContainer();
    std::string getSpriteContainerKey();
    std::string spriteContainerKey;
    bool hasSprite();
    sf::Color color;
    
    bool isDead() { return _isDead; }
    bool toRemove() { return _toRemove; }
    
protected:
    std::vector<CGuiText*> _GuiTextVector;
    void _cleanUpTextVector();
    
    // Remember to allways call your parents _doLogic, renderAdditional and _collisionLogic function.
    virtual void _doLogic();
    virtual bool _collisionLogic(CEntity* target, CollisionSides collisionSides);
    
    bool _isDead;
    bool _toRemove;
    
private:
    bool _collision(int x, int y, std::map<std::string, CEntity*>* entities);
    
};

#endif /* defined(__Third__CEntity__) */
