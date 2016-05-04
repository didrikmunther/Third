-- BinderController.lua

local BinderController = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.chatCtrl = nil
        self.binds = {}
        self.commands = {}
        self.chatBinds = {}
    end
)

function BinderController:onComponentAdd(component)
    if(component == "Standard/ChatController") then
        self.chatCtrl = self.parent:getComponent(component)
        self.chatCtrl:registerCommand("bind", self)

        self:loadBinds("binds.cfg")
    end
end

function BinderController:loadBinds(file)
    file = "resources/config/" .. file
    decoded = json.decode(game.readFile(file))

    if(decoded["binds"]) then
        for k,v in pairs(decoded["binds"]) do
            for k2,v2 in pairs(v) do
                self.binds[KeyCode[k2]] = v2
            end
        end
    end

    if(decoded["commands"]) then
        for k,v in pairs(decoded["commands"]) do
            for k2,v2 in pairs(v) do
                self.commands[KeyCode[k2]] = v2
            end
        end
    end

    if(decoded["chatbinds"]) then
        for k,v in pairs(decoded["chatbinds"]) do
            for k2,v2 in pairs(v) do
                self.chatBinds[k2] = v2
                commands = {}
                for w in k2:gmatch("%S+") do table.insert(commands, w) end

                if(not self.chatCtrl.registered[commands[1]]) then
                    self.chatCtrl:registerCommand(commands[1], self)
                end
            end
        end
    end
end

function BinderController:unloadBinds(file)
    file = "resources/config/" .. file
    decoded = json.decode(game.readFile(file))

    if(decoded["binds"]) then
        for k,v in pairs(decoded["binds"]) do
            for k2,v2 in pairs(v) do
                self.binds[KeyCode[k2]] = nil
            end
        end
    end

    if(decoded["commands"]) then
        for k,v in pairs(decoded["commands"]) do
            for k2,v2 in pairs(v) do
                self.commands[KeyCode[k2]] = nil
            end
        end
    end

    if(decoded["chatbinds"]) then
        for k,v in pairs(decoded["chatbinds"]) do
            for k2,v2 in pairs(v) do
                self.chatBinds[k2] = nil
            end
        end
    end
end

function BinderController:onChatCommand(commands)
    if(commands[1] == "bind") then
        if(not commands[2] or not commands[3]) then do return end end
        
        self.binds[KeyCode[commands[2]]] = commands[3]
    end

    str = ""
    i = 1
    while(commands[i]) do
        str = str .. " " .. commands[i]
        i = i + 1
    end
    str = string.sub(str, 2)

    if(self.chatBinds[str]) then
        self.chatCtrl:parse("/lua " .. self.chatBinds[str])
    end
end

function BinderController:onEvent(key, keyDown)
    if(not keyDown or self.component.instance.game.ignoreEvents) then do return end end

    if(self.binds[key]) then
        self.chatCtrl:onChatCommand({"lua", self.binds[key]})
    end
    if(self.commands[key]) then
        self.chatCtrl:parse("/" .. self.commands[key])
    end
end

function BinderController:onKeyStates(state)
    if(game:leftMousePressed()) then
        self:onEvent("LEFTMOUSEDOWN", true)
    end
    if(game:rightMousePressed()) then
        self:onEvent("RIGHTMOUSEDOWN", true)
    end
end

function create(parent, component)
    return BinderController(parent, component)
end