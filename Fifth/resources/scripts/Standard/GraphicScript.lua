-- GraphicScript.lua

local GraphicScript = class(
    function(self, parent, component)

        self.parent = parent
        self.component = component

        parent:addComponent(self.component.instance, game.getScript("Standard/ExplodeOnDeath"))
        parent:addProperty(EntityProperty.STATIC)
        parent:removeProperty(EntityProperty.COLLIDABLE)
        parent.defaultSprite = "lua"

        self.isTargeted = false
        self.targetedScript = nil
        self.deserialize = "{}"
                            
        self.offsetX = 0
        self.offsetY = 0

    end
)

function GraphicScript:setScript(script)
    self.targetedScript = script
end

function GraphicScript:onLoop()
    if(self.isTargeted) then
        body = self.parent.body
        x, y = self.component:getRelativeMouse()
        
        body:setPosition(x - self.offsetX, y - self.offsetY)
    end
end

function GraphicScript:onSerialize()
    c = self.component

    if(self.targetedScript ~= nil) then
        c:addString("script", self.targetedScript:getName())
    end
end

function GraphicScript:onDeserialize(value)
    d = json.decode(value)

    if(d.script ~= nil) then
        self.targetedScript = getVal(self.targetedScript, game.getScript(d.script))
    end
end

function GraphicScript:onClick()
    x, y = self.component:getRelativeMouse()
    x2, y2 = self.component:getMouse()
    box = self.parent.body.box

    if(self.isTargeted) then
        entity = self.parent.entityManager:getEntityAtCoordinate(x, y, self.parent)
        if(entity ~= nil) then
            if(self.targetedScript ~= nil) then
                entity:addComponent(self.component.instance, self.targetedScript)
                component = entity:getComponent(self.targetedScript:getName())
                if(component ~= nil and component.onDeserialize ~= nil) then
                    component:onDeserialize(self.deserialize)
                end
            end
            self.parent.isDead = true
        end
    else
        self.offsetX = x - box.x
        self.offsetY = y - box.y
    end

    self.isTargeted = not self.isTargeted

end

function GraphicScript:onRender()
    box = self.parent.body.box
    name = "UNKNOWN SCRIPT"
    camera = self.component.camera

    if(self.targetedScript) then
        name = self.targetedScript:getName()
    end

    self.component:renderText(box.x - camera:offsetX(), box.y + box.h + 2 - camera:offsetY(), 1, name, "TESTFONT", 255, 255, 255)
end

function create(parent, component)
    return GraphicScript(parent, component)
end

