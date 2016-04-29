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
        self.maxBufferSize = 100000000
                              
        self.shift = false
        self.currentPos = 0
                              
        self.registered = {}
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

-- command: str, component: LuaComponentObj
function ChatController:registerCommand(command, component)

    if(self.registered[command] ~= nil) then
        game.log(LogType.ERROR, "Could not register command \"" .. command .. "\" because it already exists")
    else
        self.registered[command] = component
    end
end

function ChatController:parse(input)
    if(string.sub(input, 1, 1) == "/") then
        input = string.sub(input, 2, string.len(input))

        commands = {}
        for w in input:gmatch("%S+") do table.insert(commands, w) end

        -- PARSING

        if(commands[1] == "restart") then self.component.instance.game:restart() end

        if(commands[2] ~= nil) then
            if(commands[1] == "script") then
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

            if(commands[1] == "lua") then
                len = table.getn(commands)
                result = ""
                for i = 2,len do
                    result = result .. commands[i] .. " "
                    end
                line = "return function(self) " .. result .. " end"
                if(not pcall(function() func = loadstring(line)() end) or not pcall(function() func(self) end)) then
                    print("Error in executing: " .. result)
                end
            end

            if(commands[1] == "load") then
                self.component.instance:loadLevel(commands[2])
            end
            
            if(commands[1] == "save") then
                path = "resources/level/" .. commands[2]
                serialized = self.component.instance:onSerialize()
                print(serialized)
                game.clearFile(path)
                game.writeToFile(path, serialized)
            end
        end
        
        comp = self.registered[commands[1]]
        if(comp ~= nil) then
            comp:onCommand(commands)
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

function ChatController:onTextInput(input)
    if(self.isTyping) then
        if(string.len(self.buffer) < self.maxBufferSize) then
            self.buffer = self.buffer .. input
        end
    end

end

function ChatController:onEvent(key, keyDown)
    if(not keyDown) then do return end end

    if(key == KeyCode._RETURN or key == KeyCode._SLASH) then
        if(key == KeyCode._SLASH) then
            self.isTyping = true
        else
            self.isTyping = not self.isTyping
        end

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

    elseif(key == KeyCode._ESCAPE) then
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
