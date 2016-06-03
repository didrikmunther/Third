-- GUIWindow.lua

local GUIWindow = class(
    function(self, parent, component)
        self.parent = parent
        self.component = component
    end
)



function create(parent, component)
    return GUIWindow(parent, component)
end