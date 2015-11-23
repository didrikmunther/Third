//
//  CEntityManager.h
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CEntityManager__
#define __Third__CEntityManager__

#include <stdio.h>
#include <memory>
#include <vector>
#include <map>
#include <string>

#include "Define.h"
#include "NSurface.h"
#include "CSerializable.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


class CEntity;
class CSprite;
class CGuiText;
class CBackground;
class CCamera;
class CWindow;
class CInstance;
class lua_State;
class CGame;

class CEntityManager : public CSerializable {
    
public:
    CEntityManager();
    
    CEntity* getEntity(std::string name);
    CEntity* getEntityAtCoordinate(int x, int y);
    std::string getNameOfEntity(CEntity* entity);
    CBackground* getBackground(std::string name);
    std::string addEntity(CEntity* entity, std::string name = "");
    void addParticle(CEntity* particle);
    void addGuiText(CGuiText* guiText);
    void addBackground(std::string name, CBackground* background);
    
    void onLoop(CInstance* instance);
    void onEvent(CInstance* instance, int key, bool keyDown);
    void onKeyStates(CInstance* instance, const Uint8* keystates);
    void onRender(CWindow* window, CCamera* camera);
    void onCleanup();
    
    void onSerialize(rapidjson::Value* value, rapidjson::Document::AllocatorType* alloc, CInstance* instance);
    void onDeserialize(rapidjson::Value* value, CInstance* instance);
    
    CEntity* createEntity(Box box, Color color);
    CEntity* createEntity(Box box, std::string spriteKey);
    
    int pushEntities(lua_State* L);
    
    void entityCleanup();
    void particleEmitterCleanup();
    void particleCleanup();
    void guiTextCleanup();
    
    int entityID;
    
    int renderFlags;
    void addRenderFlag(RenderFlags renderFlag);
    void removeRenderFlag(RenderFlags renderFlag);
    void toggleRenderFlag(RenderFlags renderFlag);
    
    void addRenderFlag(int renderFlag) { addRenderFlag((RenderFlags)renderFlag); }
    void removeRenderFlag(int renderFlag) { removeRenderFlag((RenderFlags)renderFlag); }
    void toggleRenderFlag(int renderFlag) { toggleRenderFlag((RenderFlags)renderFlag); }
    
private:
    std::map<std::string, CEntity*> _entities;
    std::vector<CEntity*> _particles;
    std::vector<CGuiText*> _guiTextElements;
    std::map<std::string, CEntity*> _deadEntities;
    std::map<std::string, CBackground*> _backgrounds;
    
    int _gridSize;
    
};

#endif /* defined(__Third__CEntityManager__) */
