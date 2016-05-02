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
    decoded = json.decode(game.readFile("resources/config/binds.cfg"))

    for k,v in pairs(decoded["binds"]) do
        for k2,v2 in pairs(v) do
            self.binds[KeyCode[k2]] = v2
        end
    end

    for k,v in pairs(decoded["commands"]) do
        for k2,v2 in pairs(v) do
            self.commands[KeyCode[k2]] = v2
        end
    end
end

function BinderController:onComponentAdd(component)
    if(component == "Standard/ChatController") then
        self.chatCtrl = self.parent:getComponent(component)
        self.chatCtrl:registerCommand("bind", self)
    end
end

function BinderController:onChatCommand(commands)
    if(commands[1] == "bind") then
        if(not commands[2] or not commands[3]) then do return end end
        
        self.binds[KeyCode[commands[2]]] = commands[3]
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

function create(parent, component)
    return BinderController(parent, component)
end