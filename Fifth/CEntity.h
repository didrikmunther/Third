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
#include <memory>
#include <map>
#include <vector>
#include "CBody.h"
#include "CSprite.h"
#include "CAssetManager.h"
#include "CRenderable.h"
#include "CWindow.h"
#include "CChatBubble.h"
#include <unordered_map>
//#include "CInstance.h"

struct GridCoordinates {
    int x, y;
};

class CGuiText;
class CCamera;
class CEntityManager;
class EComponent;
class CInstance;

class CEntity : public CRenderable {
    
public:
    CEntity(Box rect, SDL_Color color);
    CEntity(Box rect, std::string spriteContainerKey);
    ~CEntity();
    
    EntityTypes entityType;
    
    void init();
    void onLoop(CInstance* instance);
    void afterLogicLoop(std::vector<CEntity*>* entities, CInstance* instance); // After the velocities have been added, move the entity
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
    int collisionLayer;
    bool isOnCollisionLayer(int collisionLayer);
    void toggleCollisionLayer(int collisionLayer) { this->collisionLayer ^= collisionLayer; }
    void addCollisionLayer(int collisionLayer) { this->collisionLayer |= collisionLayer; }
    void removeCollisionLayer(int collisionLayer) { if(isOnCollisionLayer(collisionLayer)) toggleCollisionLayer(collisionLayer); }
    
    void move(std::vector<CEntity*>* entities, CInstance* instance);
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
    SDL_Color color;
    
    CBody body;
    
    int properties;
    bool hasProperty(int property);
    void toggleProperty(int property);
    void addProperty(int property);
    void removeProperty(int property);
    
    void addGuiText(CGuiText* text) { _GuiTextVector.push_back(text); }
    
    void addComponent(EComponent* component);
    template<typename T>
    T* getComponent() {
        if(_components.count(&typeid(T)) != 0)
        {
            return static_cast<T*>(_components[&typeid(T)]);
        }
        else
        {
            return nullptr;
        }
    }
    void _clearComponents();
    
    bool hasMoved() { return _hasMoved; };
    bool isDead;
    bool toRemove;
    
    std::vector<GridCoordinates> gridCoordinates;
    
protected:
    std::unordered_map<const std::type_info*, EComponent*> _components;
    
    std::vector<CGuiText*> _GuiTextVector;
    void _cleanUpTextVector();
    
    bool _collisionLogic(CEntity* target, CInstance* instance, CollisionSides collisionSides);
    
private:
    bool _collision(int x, int y, std::vector<CEntity*>* entities, CInstance* instance);
    
    bool _hasMoved;
    
};

#endif /* defined(__Third__CEntity__) */
