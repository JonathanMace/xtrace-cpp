print("Requiring luaxtrace")
local xtracer = require 'luaxtrace'

local tracing = xtracer.IsTracing()
if tracing ~= true then
  xtracer.StartLuaTrace("LuaTest")
end
print("Logging now")
xtracer.LogXTrace("Well fancy seeing you here")

os.execute("sleep " .. 5)
