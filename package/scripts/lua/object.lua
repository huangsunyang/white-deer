
local NativeObjectMeta = {
    __index = function(t, key)
        local instance = rawget(t, "__instance")
        local parent = rawget(t, "__parent")
        if instance == nil then
            return parent and parent[key]
        end

        local value = instance[key]
        if value == nil then
            return parent and parent[key]
        end

        if type(value) ~= "function" then
            return value
        end

        -- function value should pass __instance as first arg
        return rawset(t.__parent or t, key, function(t, ...)
            return value(t.__instance, ...)
        end)[key]
    end,

    __newindex = function(t, key, value)
        local instance = rawget(t, "__instance")
        local metatable = getmetatable(instance)
        if instance and metatable and metatable[key] then
            instance[key] = value
        else
            rawset(t, key, value)
        end
    end
}


NativeObject = {
    new = function(self, t)
        if t == nil then
            t = {}
        end

        t.__parent = self
        return setmetatable(t, NativeObjectMeta)
    end,
}

return setmetatable(NativeObject, NativeObjectMeta)
