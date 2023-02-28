require "object"

require("LuaPanda").start("127.0.0.1", 8818);

F16Behavior = NativeObject:new({
    HOT_RELOAD = true,
    update = 1
})

function F16Behavior:Awake()
    self.gameObject = self:GetGameObject()
    self.transform = self:GetGameObject():GetTransformComponent()
    self.position = self.transform.position
    self.count = 0
end

function F16Behavior:Update()
    self.count = (self.count + 1) % 100
    local delta = self.count < 50 and 0.03 or -0.03

    self.position.z = self.position.z + delta
    self.transform.position = self.position
end

return F16Behavior
