-- ChatController.lua

local ChatController = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.isTyping = false
        self.buffer = ""

        self.width = 50
        self.initWidth = self.width
        self.height = 40
        self.maxBufferSize = 52
                              
        self.shift = false
    end
)

function ChatController:isKeyCode(key)
    for k, v in pairs(KeyCode) do
        if(v == key) then
            return true
        end
    end

    return false
end

function ChatController:parse(input)
    if(string.sub(input, 1, 1) == "/") then
        input = string.sub(input, 2, string.len(input))

        commands = {}
        for w in input:gmatch("%S+") do table.insert(commands, w) end

        -- PARSING

        if(commands[1] == "restart") then self.component.instance.game:restart() end

        if(commands[1] == "script" and commands[2] ~= nil) then
            comp = commands[2]
            mX, mY = self.component:getRelativeMouse()
            temp = self.parent.entityManager:createSpriteEntity(Box(mX, mY, 16 * 4, 16 * 4), "lua")
            self.parent.entityManager:addEntity(temp, "")
            temp:addComponent(self.component.instance, game.getScript("Standard/GraphicScript"))
            graphic = temp:getComponent("Standard/GraphicScript")
            graphic:setScript(game.getScript(comp))
            if(commands[3] ~= nil) then
                graphic.deserialize = commands[3]
            end
        end

        return ""
    end

    return input
end

function ChatController:onKeyStates(state)
    if(state:hasState(ScanCode._RSHIFT) or state:hasState(ScanCode._LSHIFT)) then
        self.shift = true
    else
        self.shift = false
    end
end

function ChatController:onEvent(key, keyDown)
    if(not keyDown) then do return end end

    if(key == KeyCode._RETURN) then
        self.isTyping = not self.isTyping

        if(not self.isTyping) then -- player has typed something
            toSay = self:parse(self.buffer)
            if(toSay ~= "") then
                self.component.instance.player:say(toSay, "TESTFONT", ChatBubbleType.SAY)
            end
            self.buffer = ""
            self.component.instance.game.ignoreEvents = false
            do return end
        else
            self.component.instance.game.ignoreEvents = true
        end

    elseif(key == KeyCode._BACKSPACE) then
        if(not self.isTyping) then do return end end

        self.buffer = string.sub(self.buffer, 1, -2)

    elseif(self.isTyping and key ~= KeyCode._ESCAPE) then
        if(key <= 1024) then -- temporary to ignore modifier key
            toAdd = string.char(key)
            if(self.shift) then
                toAdd = string.upper(toAdd)
            end
            if(string.len(self.buffer) < self.maxBufferSize) then
                self.buffer = self.buffer .. toAdd
            end
        end

    end

    if(self.isTyping and key == KeyCode._ESCAPE) then
        self.component.instance.game.isRunning = false

    end
end

function ChatController:onRenderAdditional()
    if(self.isTyping) then
        x = 10
        y = game:getScreenHeight() - self.height - 10
        w = self.width
        h = self.height
        r = 2

        self.component:renderRect(x, y, w, h, 100, 100, 100, 225)

        self.component:renderRect(x,        y,          r,      h, 255, 0, 0, 225)
        self.component:renderRect(x,        y,          w,      r, 255, 0, 0, 225)
        self.component:renderRect(x + w - r,y,          r,      h, 255, 0, 0, 225)
        self.component:renderRect(x,        y + h - r,  w,      r, 255, 0, 0, 225)

        x = x + 5
        y = y + 6

        self.component:renderText(x, y, 1, self.buffer, "TESTFONT", 255, 255, 255)
        self.width = self.initWidth + (14 * string.len(self.buffer))
    end

    if (self.isTyping == false) then
        self.width = self.initWidth
    end
end

function create(parent, component)
    return ChatController(parent, component)
end
