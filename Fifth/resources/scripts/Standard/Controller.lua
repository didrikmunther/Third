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

    self.component.instance:loadAssets("testMap2.map")
    self.component.instance:loadAssets("testMap1.map")

    --self.component.instance:loadLevel("testlevel.lvl")
end

function create(parent, component)
    return Controller(parent, component)
end