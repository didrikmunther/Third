-- BinderController.lua

local BinderController = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component

        self.chatCtrl = nil
        self.binds = {}
        self.commands = {}
    end
)



function BinderController:onInit()
    self:loadBinds("resources/config/adminbinds.cfg")
    --self:loadBinds("resources/config/userbinds.cfg")
end

function BinderController:onComponentAdd(component)
    if(component == "Standard/ChatController") then
        self.chatCtrl = self.parent:getComponent(component)
        self.chatCtrl:registerCommand("bind", self)
        self.chatCtrl:registerCommand("mode", self)
    end
end

function BinderController:loadBinds(file)
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
end

function BinderController:unloadBinds(file)
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
end

function BinderController:onChatCommand(commands)
    if(commands[1] == "bind") then
        if(not commands[2] or not commands[3]) then do return end end
        
        self.binds[KeyCode[commands[2]]] = commands[3]
    end
    if(commands[1] == "mode" and commands[2]) then
        mode = commands[2]
        self:unloadBinds("resources/config/adminbinds.cfg")
        self:unloadBinds("resources/config/userbinds.cfg")
        if(mode == "admin") then
            self:loadBinds("resources/config/adminbinds.cfg")
        else -- mode == self.USER
            self:loadBinds("resources/config/userbinds.cfg")
        end
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