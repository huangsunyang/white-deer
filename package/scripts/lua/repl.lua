--[[
]]
function is_utf8(s)
    -- todo: utf8
    do return true end

	for i, valid in utf8.byte_indices(s) do
		if not valid or not utf8.isvalid(s, i) then
			return false
		end
	end
	return true
end

function ToString(t, limit, level)
	if limit == nil then limit = - 1 end

	if type(t) ~= "table" or limit == 0 then
		local success, result = pcall(tostring, t)
		if not success or type(result) ~= "string" or not is_utf8(result) then return "[cannot convert to string]" end
		if type(t) == "string" then return '"' .. result .. '"' end
		if type(t) == "function" then
			local funcInfo = debug.getinfo(t, 'S')
			return '<' .. result .. '> :' ..(funcInfo.linedefined or nil) .. '@' ..(funcInfo.short_src or "nil")
		end
		return result
	end
	if limit > 0 then limit = limit - 1 end

	if _EnterPrintTableArea(t) > 0 then
		_LeavePrintTableArea(t)
		return "{...}"
	end

	level = level or 0
	local ret = "{\n"

	local nameToIndex = _IsOPTable(t)
	local indexToName = setmetatable({}, {__index = function(t, k) return k end})

	local tableSize = 0
	for k, v in pairs(nameToIndex or {}) do
		tableSize = tableSize + 1
		indexToName[v] = k
	end

	if not nameToIndex then
		for k, v in pairs(t) do
			tableSize = tableSize + 1
			if tableSize > 30 then break end
		end
	end

	if tableSize > 30 then limit = 0 end

	local stringTable = {}
	if not nameToIndex then
		tableSize = 0
		for k, v in pairs(t) do
			tableSize = tableSize + 1
			table.insert(stringTable, string.rep('  ', level + 1) .. ToString(indexToName[k], limit) .. " = " .. ToString(v, limit, level + 1) .. ',\n')
			if tableSize >= 100 then
				table.insert(stringTable, string.rep('  ', level + 1) .. '...... table size too big, only show 100 items\n')
				break
			end
		end
	else
		for i = 1, tableSize do
			local k, v = i, t[indexToName[i]]
			local line = string.format("%s%s(%s)=%s,\n", string.rep('  ', level + 1), ToString(indexToName[k], limit), k, ToString(v, limit, level + 1))
			table.insert(stringTable, line)
			if i >= 100 then
				table.insert(stringTable, string.rep('  ', level + 1) .. '...... table size too big, only show 100 items\n')
				break
			end
		end
	end

	_LeavePrintTableArea(t)
	return ret .. table.concat(stringTable, "") .. string.rep('  ', level) .. "}"
end

function _IsOPTable(t)
    do return false end
	local indexFunc =(getmetatable(t) or {}).__index
	if type(indexFunc) ~= "function" then
		indexFunc = nil
	end
	return getfenv(indexFunc).Types
end

local printingTables = {}

function _EnterPrintTableArea(t)
	local old = printingTables[t] or 0
	printingTables[t] = old + 1
	return old
end

function _LeavePrintTableArea(t)
	printingTables[t] = printingTables[t] - 1
end

function printD(t)
	print(ToString(t))
end

function ShowTrace(func)
	local function wrapper(...)
		print(debug.traceback())
		func(...)
	end
	return wrapper
end

function TraceClass(cls)
	for k, v in pairs(cls) do
		if type(v) == "function" then
			cls[k] = ShowTrace(v)
		end
	end
end

function CodeRun(str)
	local leftValueName = string.match(str, "^%s*([%w_%.%[%]]+)%s*=[^=]+.*")
	if leftValueName == nil then
		leftValueName = string.match(str, "^local%s*([%w_%.%[%]]+)%s*=[^=]+.*")
	end

	if leftValueName == nil then
		leftValueName = string.match(str, "^%s*function%s*([%w_%.:]+)%s*%(")
		if leftValueName ~= nil then
			leftValueName = string.gsub(leftValueName, ":", ".")
		end
	end

	local funcStrCandidate = {}
	if not leftValueName then
		table.insert(funcStrCandidate, string.format("return (%s)", str))
	else
		table.insert(funcStrCandidate, string.format("%s\nreturn %s", str, leftValueName))
	end
	table.insert(funcStrCandidate, str)

	local success, ret_val
	for k, v in ipairs(funcStrCandidate) do
		success, ret_val = pcall(loadstring or load, v)
		if success and ret_val then
			success, ret_val = pcall(ret_val)
			if success then _ = ret_val end
			break
		else
			ret_val = "loadstring failed"
		end
	end
	return ToString(ret_val, 3)
end

function CodeComplete(originStr)
	local candidates = {}
	local str = string.match(originStr, "[%w_][%[%]%w_%.:]*$") or ""
	if(str == "") then return {candidates = candidates, predict = originStr} end

	local last = string.match(str, "([%.:][%w_%[%]]*)$")
	local cur_env, split, pre, success, func = _G, '.', str

	if last then
		split, pre = last:sub(1, 1), last:sub(2)
		local expr = str:sub(1, str:len() - 1 - pre:len())

		success, func = pcall(loadstring, "return (" .. expr .. ")")
		cur_env =(success and type(func) == "function" and func()) or {}
	else
		cur_env, split, pre = _G, '.', str
	end

	repeat
		_InsertEnvKeyWithPrefix(cur_env, candidates, pre, split == ':')
		if _IsWrapperTypeInfo(cur_env) or _IsSpecialType(cur_env) then
			cur_env = getmetatable(cur_env)
		else
			cur_env =(getmetatable(cur_env) or {}).__index
		end
	until not cur_env or _VisitedEnv(cur_env)
	_ClearVisitedEnv()

	candidates = RemoveRepeated(candidates)
	table.sort(candidates)
	local commonPrefix = CommonPrefix(candidates)
	if #commonPrefix < #pre then commonPrefix = pre end
	pre = string.gsub(pre, "[%[%]]", "%%%1")
	local predict = string.gsub(originStr, pre .. '$', commonPrefix)
	return {candidates = candidates, predict = predict}
end

function _SafeIndex(cur_env, index)
	local __SafeIndex = function()
		return cur_env[index]
	end
	local success, ret = pcall(__SafeIndex)
	return success and ret
end

function _IsWrapperTypeInfo(cur_env)
	return _SafeIndex(cur_env, ".name") and _SafeIndex(cur_env, ".ref")
end

function _IsSpecialType(cur_env)
	if type(cur_env) == 'table' then
		local metatable = getmetatable(cur_env)
		return rawequal(metatable, UnityEngine.Vector2) or rawequal(metatable, UnityEngine.Vector3) or rawequal(metatable, UnityEngine.Vector4)
	end
end

visitedEnv = {}

function _InsertEnvKeyWithPrefix(cur_env, candidates, pre, onlyFunction)
	if type(cur_env) ~= "table" then
		return
	end
	local isWrapperTypeInfo = _IsWrapperTypeInfo(cur_env)
	for k, v in pairs(cur_env) do
		if type(k) == "string" and string.sub(k, 1, string.len(pre)) == pre then
			if onlyFunction and type(v) ~= "function" then
			else
				table.insert(candidates, k)
			end
		elseif isWrapperTypeInfo and not onlyFunction and type(k) == "userdata" and type(v) == "table" then
			_InsertEnvKeyWithPrefix(v, candidates, pre)
		end
	end

	local OPTypeTable = _IsOPTable(cur_env)
	if OPTypeTable then
		_InsertEnvKeyWithPrefix(OPTypeTable, candidates, pre, onlyFunction)
	end
	visitedEnv[cur_env] = 1
end

function _VisitedEnv(env)
	return visitedEnv[env] ~= nil
end

function _ClearVisitedEnv()
	visitedEnv = {}
end

function RemoveRepeated(candidates)
	local hash, ret = {}, {}
	for _, v in ipairs(candidates) do
		hash[v] = 1
	end

	for k, _ in pairs(hash) do
		table.insert(ret, k)
	end

	return ret
end

function CommonPrefix(strList)
	local minStrLen = - 1
	for _, str in ipairs(strList) do
		if minStrLen < 0 or #str < minStrLen then minStrLen = #str end
	end

	local prefix = ''
	for i = 1, minStrLen do
		local curChar = strList[1]:sub(i, i)
		for j = 2, #strList do
			if strList[j]:sub(i, i) ~= curChar then
				return prefix
			end
		end
		prefix = prefix .. curChar
	end
	return prefix
end
