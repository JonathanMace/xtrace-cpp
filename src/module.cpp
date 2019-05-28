#include "luaxtrace.h"
#include "lua_baggage.h"
#include <iostream>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

// Copied from Lua 5.3 so that we can use it with Lua 5.1.
static void setfuncs(lua_State* L, const luaL_Reg* l, int nup) {
  luaL_checkstack(L, nup + 1, "too many upvalues");
  for (; l->name != NULL; l++) { /* fill the table with given functions */
    int i;
    lua_pushstring(L, l->name);
    for (i = 0; i < nup; i++) /* copy upvalues to the top */
      lua_pushvalue(L, -(nup + 1));
    lua_pushcclosure(L, l->func, nup); /* closure with those upvalues */
    lua_settable(L, -(nup + 3));
  }
  lua_pop(L, nup); /* remove upvalues */
}

/** This is the function called when require "luaxtrace" is called
  *
  */
extern "C" int luaopen_luaxtrace(lua_State* L) {
  lua_newtable(L);
  const struct luaL_Reg functions[] = {
    {"IsTracing", lua_xtracer::IsTracing},
    {"StartLuaTrace", lua_xtracer::StartLuaTrace},
    {"LogXTrace", lua_xtracer::LogLocal},
    {"SetBaggage", lua_xtracer::SetBaggage},
    {"JoinBaggage", lua_xtracer::JoinBaggage},
    {"DeleteBaggage", lua_xtracer::DeleteBaggage},
    {"BranchBaggage", lua_xtracer::BranchBaggage},
    {nullptr, nullptr}
  };
  setfuncs(L, functions, 0);
  
  return 1;
}
