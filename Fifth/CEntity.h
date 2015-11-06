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
#include <sstream>

#include "CChatBubble.h"
#include "CBody.h"
#include "NFile.h"
#include "CComponent.h"
#include "NSurface.h"
#include "CCombatText.h"


class CParticle;
class CSpriteContainer;
class CGuiText;
class CCamera;
class CEntityManager;
class CWindow;
class CInstance;
class EUtility;

enum class BasicUtilities;

namespace CollisionLayers {
    enum CollisionLayers {
        LAYER0      = 1 << 0,   // 1
        LAYER1      = 1 << 1,   // 2
        LAYER2      = 1 << 2,   // 4
        LAYER3      = 1 << 3,   // 8
        LAYER4      = 1 << 4,   // 16
        LAYER5      = 1 << 5,   // 32
        LAYER6      = 1 << 6,   // 64
        LAYER7      = 1 << 7,   // 128
    };
}

namespace EntityProperty {
    enum EntityProperty {
        COLLIDABLE  = 1 << 0,
        GRAVITY_AFFECT = 1 << 1,
        HIDDEN      = 1 << 2,
        STATIC      = 1 << 3,
        FLIP        = 1 << 4,
        FLIP_FREEZED= 1 << 5
    };
}
    
namespace SpriteStateTypes {
    enum SpriteStateTypes {
        IDLE = 0,
        ASCENDING,
        DESCENDING,
        FLYING,
        WALKING,
        SNEAKING,
        RUNNING,
        DUCKING,
        TOTAL_SPRITESTATETYPES
    };
}

struct GridCoordinates {
    int x, y;
};

struct CollisionSides {
    CollisionSides() { (*this) = false; }
    
    bool top,
         bottom,
         right,
         left;
    
    void operator=(bool b) {
        top    =
        bottom =
        right  =
        left   = b;
    }
    
    void operator|=(CollisionSides b) {
        top    |= b.top;
        bottom |= b.bottom;
        right  |= b.right;
        left   |= b.left;
    }
};

class CEntity {
    
friend class CComponent;
    friend class CEntityManager;
    
public:
    CEntity(Box rect, Color color);
    CEntity(Box rect, std::string spriteContainerKey);
    ~CEntity();
    
    void init();
    void onLoop(CInstance* instance);
    void onEvent(CInstance* instance, int key, bool keyDown);
    void onKeyStates(CInstance* instance, const Uint8* keystates);
    void onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    virtual void renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags);
    
    void onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc);
    void onDeserialize(rapidjson::Value* value);
    
    int collisionLayer;
    bool isOnCollisionLayer(int collisionLayer);
    void toggleCollisionLayer(int collisionLayer) { this->collisionLayer ^= collisionLayer; }
    void addCollisionLayer(int collisionLayer) { this->collisionLayer |= collisionLayer; }
    void removeCollisionLayer(int collisionLayer) { if(isOnCollisionLayer(collisionLayer)) toggleCollisionLayer(collisionLayer); }
    
    int properties;
    bool hasProperty(int property);
    void toggleProperty(int property);
    void addProperty(int property);
    void removeProperty(int property);
    
    void move(std::vector<CEntity*>* entities, CInstance* instance);
    bool coordinateCollision(int x, int y, int w, int h, int x2, int y2, int w2, int h2);
    bool coordinateCollision(int x, int y, int w, int h);
    std::vector<GridCoordinates> gridCoordinates;
    
    CBody* body;
    CollisionSides collisionSides;
    
    void say(std::string text, std::string fontKey, int type);
    void say(std::string text, std::string fontKey, ChatBubbleType type);
    
    void setSpriteStateType(int type, std::string sprite);
    std::string spriteStateTypes[SpriteStateTypes::TOTAL_SPRITESTATETYPES];
    
    void setSpriteContainer(std::string spriteContainerKey);
    CSpriteContainer* getSpriteContainer();
    std::string getSpriteContainerKey();
    std::string spriteContainerKey;
    bool hasSprite();
    SDL_Color color;
    
    bool hasMoved() { return _hasMoved; }
    
    void addComponent(CInstance* instance, CLuaScript* script);
    CComponent* getComponent(std::string key);
    int getComponent(lua_State* L);
    
    int transparency;
    bool spriteFollowsCollisionBox;
    
    bool isDead;
    bool toRemove;
    
    CEntityManager* entityManager;

    std::vector<CGuiText*> guiTextVector;
    void addTextObject(CGuiText* text) {
        CGuiText* temp = new CGuiText(*text);
        guiTextVector.push_back(temp);
    }
    void addCombatText(CCombatText* text) {
        CCombatText* temp = new CCombatText(*text);
        guiTextVector.push_back(temp);
    }
    
    bool compare(CEntity* other) {
        return other == this;
    }
    
    CEntity* getThis() {
        return this;
    }
    
protected:
    void _cleanUpTextVector();
    void _cleanUpComponents();
    
    bool _onCollision(CEntity* target, CollisionSides* collisionSides);
    
    std::map<std::string, CComponent*> components;
    
private:
    bool _collision(int x, int y, std::vector<CEntity*>* entities);
    bool _hasMoved;
    
};

#endif /* defined(__Third__CEntity__) */
