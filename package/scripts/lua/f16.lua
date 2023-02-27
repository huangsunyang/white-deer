F16Behavior = LuaScript.new()

function F16Behavior:Update()
    print(type(self))
    if self.update == nil then self.update = 1
    else self.update = self.update + 1 end
    print(self, self.update)
end

return F16Behavior