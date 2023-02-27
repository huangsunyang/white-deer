require "object"

F16Behavior = NativeObject:new({
    update = 1
})

function F16Behavior:Update()
    g = self
    if self.update == nil then self.update = 1
    else self.update = self.update + 1 end
    print(self, self.update, self.filename)
end

return F16Behavior
