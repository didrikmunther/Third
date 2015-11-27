-- GraphicScript.lua

local GraphicScript = class(
    function(self, parent, component)

        self.parent = parent
        self.component = component


    end
)

function GraphicScript:onClick()

end



function create()
    return GraphicScript()
end

