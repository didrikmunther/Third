-- Controller.lua

local Controller = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.hasInitializedAdmin = false
    end
)

function Controller:onInit()
    self.parent:addProperty(EntityProperty.STATIC)
    self.parent:addProperty(EntityProperty.HIDDEN)
    self.parent:removeProperty(EntityProperty.COLLIDABLE)
    self.component.instance.gravity = 0.7

    self.defaultMode = "user"

    self.component.instance:loadAssets("testMap2.map")
    self.component.instance:loadAssets("testMap1.map")

    game.playMusic("bgMusic", -1)

    --self.component.instance:loadLevel("testlevel.lvl")
end

function Controller:onComponentAdd(component)
    if(component == "Standard/BinderController") then
        self.binderCtrl = self.parent:getComponent(component)
    end
end

function Controller:onLoop()
    if(self.hasInitializedAdmin) then do return end end
    if(not self.component.instance.player) then do return end end
    if(not self.binderCtrl) then do return end end
    self.binderCtrl:onChatCommand({"mode", self.defaultMode})
    self.hasInitializedAdmin = true
end

function create(parent, component)
    return Controller(parent, component)
end