-- ChatController.lua

local ChatController = class (
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.isTyping = false
        self.buffer = ""

        self.width = 400
        self.height = 40
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

function ChatController:onEvent(key, keyDown)
    if(not keyDown) then do return end end

    if(key == KeyCode._RETURN) then
        self.isTyping = not self.isTyping

        if(not self.isTyping) then -- player has typed something
            if(self.buffer ~= "") then
                self.component.instance.player:say(self.buffer, "TESTFONT", ChatBubbleType.SAY)
                self.buffer = ""
            end
            self.component.instance.game.ignoreEvents = false
            do return end
        else
            self.component.instance.game.ignoreEvents = true
        end

    elseif(key == KeyCode._BACKSPACE) then
        if(not self.isTyping) then do return end end

        self.buffer = string.sub(self.buffer, 1, -2)

    elseif(self.isTyping and self:isKeyCode(key) and key ~= KeyCode._ESCAPE) then
        self.buffer = self.buffer .. string.char(key)

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
        y = y + 5

        self.component:renderText(x, y, 1, self.buffer, "TESTFONT", 255, 255, 255)

    end
end

function create(parent, component)
    return ChatController(parent, component)
end