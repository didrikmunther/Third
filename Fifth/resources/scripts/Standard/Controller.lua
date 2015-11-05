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
end

function Controller:onEvent(key, keyDown)
    if(keyDown) then
        if(key == KeyCode._3) then -- Collision borders
            self.parent.entityManager:toggleRenderFlag(RenderFlags.COLLISION_BORDERS)
        end

        if(key == KeyCode._5) then -- Restart game
            self.component.instance.game:restart()
        end

        if(key == KeyCode._6) then -- Random string for player to say
            chars = "abcdefghijklmnopqrstuvxyz                "
            toSay = ""
            for i = 0, 20 do
                rand = math.random(string.len(chars))
                toSay = toSay .. string.sub(chars, rand, rand)
            end

            player:say(toSay, "TESTFONT", ChatBubbleType.YELL)
        end
    end
end

function create(parent, component)
    return Controller(parent, component)
end