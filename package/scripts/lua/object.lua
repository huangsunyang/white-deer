NativeObject = {
    new = function(self, t)
        if t == nil then
            t = {}
        end

        return setmetatable(t, {
            __index = self,
            __newindex = self
        })
    end
}

local NativeObjectMeta = {
    __index = function(t, key)
        local instance = rawget(t, "__instance")
        if instance == nil then
            return nil
        end
        local value = instance[key]

        if type(value) ~= "function" then
            return value
        end

        -- function value should pass __instance as first arg
        return rawset(t, key, function(t, ...)
            return value(t.__instance, ...)
        end)[key]
    end,

    __newindex = function(t, key, value)
        -- if native value?
        local instance = rawget(t, "__instance")
        if instance and getmetatable(instance) and getmetatable(instance)[key] then
            rawget(t, "__instance")[key] = value
        else
            rawset(t, key, value)
        end
    end
}

return setmetatable(NativeObject, NativeObjectMeta)
