print("Requiring luaxtrace")
local xtracer = require 'luaxtrace'

local tracing = xtracer.IsTracing()
if tracing ~= true then
  xtracer.StartLuaTrace("LuaTest")
end
print("Logging now")
xtracer.LogXTrace("Well fancy seeing you here")

local baggage = xtracer.BranchBaggage()

print(baggage)
print(string.len(baggage))

xtracer.DeleteBaggage()

xtracer.SetBaggage(baggage)

xtracer.LogXTrace("paastav")

os.execute("sleep " .. 5)
