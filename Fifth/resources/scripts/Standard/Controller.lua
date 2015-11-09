-- Controller.lua

local Controller = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component
    end
)

function Controller:onInit()
    self.parent:addProperty(EntityProperty.STATIC)
    self.parent:addProperty(EntityProperty.HIDDEN)
    self.parent:removeProperty(EntityProperty.COLLIDABLE)
    self.component.instance.gravity = 0.3

    self.component.instance:loadMap("resources/map/testMap1.map")
end

function Controller:onEvent(key, keyDown)
    if(self.component.instance.game.ignoreEvents) then do return end end

    if(keyDown) then
        if(key == KeyCode._0) then -- Camera sway
            self.component.instance.camera.cameraSway = self.component.instance.camera.cameraSway + 10
        end

        if(key == KeyCode._9) then
            camera = self.component.instance.camera
            if(camera.cameraSway <= 10) then
                camera.cameraSway = 1
            else
                camera.cameraSway = camera.cameraSway - 10
            end
        end

        if(key == KeyCode._3) then -- Collision borders
            self.parent.entityManager:toggleRenderFlag(RenderFlags.COLLISION_BORDERS)
        end

        if(key == KeyCode._5) then -- Restart game
            self:restart()
        end

        if(key == KeyCode._6) then -- Random string for player to say
            chars = "abcdefghijklmnopqrstuvxyz                "
            toSay = ""
            for i = 0, 20 do
                rand = math.random(string.len(chars))
                toSay = toSay .. string.sub(chars, rand, rand)
            end

            self.component.instance.player:say(toSay, "TESTFONT", ChatBubbleType.YELL)
        end

        if(key == KeyCode._m) then -- Render collision area
            self.parent.entityManager:toggleRenderFlag(RenderFlags.COLLISION_AREA)
        end

        if(key == KeyCode._n) then
            self.parent.entityManager:toggleRenderFlag(RenderFlags.COLLISION_GRID)
        end

        if(key == KeyCode._b) then
            self.parent.entityManager:toggleRenderFlag(RenderFlags.ENTITY_GRID)
        end

        if(key == KeyCode._x) then
            self.parent.entityManager:toggleRenderFlag(RenderFlags.RENDER_COMBAT_TEXT)
        end

        if(key == KeyCode._7) then
            self.component.instance.camera:addCameraShake(100)
        end

        if(key == KeyCode._RIGHTBRACKET) then
            self.component.instance.gravity = 0.3
        end

        if(key == KeyCode._LEFTBRACKET) then
            self.component.instance.gravity = 0
        end

        if(key == KeyCode._j) then -- Create an npc entity
            mX, mY = self.component:getRelativeMouse()
            temp = self.parent.entityManager:createSpriteEntity(Box(mX, mY, 80, 140), "playerPink")
            self.parent.entityManager:addEntity(temp, "")
            temp.spriteFollowsCollisionBox = false
            temp:setSpriteStateType(SpriteStateTypes.ASCENDING, "playerPinkRunning")
            temp:setSpriteStateType(SpriteStateTypes.DESCENDING, "playerPinkRunning")
            temp:addComponent(self.component.instance, game.getScript("Standard/Living"))
            temp:addComponent(self.component.instance, game.getScript("Standard/Npc"))
            temp:addComponent(self.component.instance, game.getScript("Standard/Movable"))
            temp:getComponent("Standard/Npc").target = self.component.instance.player
            temp:getComponent("Standard/Movable"):onDeserialize('{"walking_movement_speed":3.0, "jumpPower":5.0}')

        end

        if(key == KeyCode._l) then -- Create a blue block
            mX, mY = self.component:getRelativeMouse()
            temp = self.parent.entityManager:createColoredEntity(Box(mX, mY, 40, 40), Color(0, 0, 255, 255))
            self.parent.entityManager:addEntity(temp, "")
            temp:addProperty(EntityProperty.STATIC)
        end
    end
end

function Controller:restart()
    self.component.instance.game:restart()
end

function create(parent, component)
    return Controller(parent, component)
end