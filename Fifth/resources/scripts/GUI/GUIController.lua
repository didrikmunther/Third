-- GUIController.lua

local GUIController = class(
	function(self, parent, component)
		self.parent = parent
		self.component = component

        self.chatCtrl = nil

        self.skins = {
            StandardGUI = 0
        }

        self.windows = {}
        self.windowID = 1
        self.windowsToRemove = {}

	end
)

function GUIController:onComponentAdd(component)
    if(component == "Standard/ChatController") then
        self.chatCtrl = self.parent:getComponent(component)
        self.chatCtrl:registerCommand("gui", self)
    end
end

function GUIController:onChatCommand(commands)
    if(commands[1] == "gui") then
        if(not commands[2]) then do return end end
    end

    if(commands[2] == "window") then
        if(commands[3] == nil) then
            self:createWindow("StandardGUI")
        else
            self:createWindow(commands[3])
        end
    end
end

function GUIController:initSkin(skin)
    if(self.skins[skin] == nil or self.skins[skin] == 1) then do return end end

    spriteSheet = game.getSpriteSheet(skin) --self.component:addSpriteSheet("GUI" .. skin, self.skins[skin][self.PATH])

    addSprite = game.addSprite
    createSprite = game.createSprite
    entityManager = self.parent.entityManager
    for i = 0, 49 do
        addSprite(createSprite(spriteSheet, Box((i % 7) * 8, math.floor(i / 7) * 8, 8, 8)), "GUI" .. skin .. i)
        --print("GUI" .. skin .. i)
    end

    self.skins[skin] = 1   
end

function GUIController:createWindow(skin)

    self:initSkin(skin)

    entityManager = self.parent.entityManager
    entity = entityManager:createColoredEntity(Box(0, 0, 0, 0), Color(0, 0, 0, 255))

    entity:addComponent(self.component.instance, game.getScript("GUI/GUIWindow"))
    entityManager:addEntity(entity, "3:GUIWindow-" .. self.windowID)

    component = entity:getComponent("GUI/GUIWindow")
    component.guiController = self

    table.insert(self.windows, component)
    self.windowID = self.windowID + 1

end

function GUIController:closeWindow(window)
    for k,v in pairs(self.windows) do
        if v == window then
            table.insert(self.windowsToRemove, k)
            return
        end
    end
end

function GUIController:collides(x, y, w, h, x2, y2)
    if x2 < x then return false end
    if x2 > x + w then return false end
    if y2 < y then return false end
    if y2 > y + h then return false end

    return true
end

function GUIController:windowCollides(window, x, y)
    wX = window.x
    wY = window.y
    wW = (window.w + 4) * window.size
    wH = (window.h + 4) * window.size

    return self:collides(wX, wY, wW, wH, x, y)

end

function GUIController:onLoop()
    for k,v in pairs(self.windowsToRemove) do
        self.windows[v].parent.toRemove = true
        table.remove(self.windows, v)
    end
    
    self.windowsToRemove = {}
end

function GUIController:onClick()
    x, y = self.component:getMouse()

    for k,v in pairs(self.windows) do
        if self:windowCollides(v, x, y) then
            v:onClick(x, y)
            break
        end
    end
end

function create(parent, component)
	return GUIController(parent, component)
end