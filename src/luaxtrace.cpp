#include "luaxtrace.h"
#include <vector>
#include <string>
#include <iostream>
#include "xtrace.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace lua_xtracer {

void RegisterXtraceLua(lua_State* L) {
	lua_register(L, "StartLuaTrace", StartLuaTrace);
	lua_register(L, "IsTracing", IsTracing);
}

int StartLuaTrace(lua_State* L) {
	int n = lua_gettop(L);

	if (n == 0) {
		XTrace::StartTrace();
	} else {
		std::vector<std::string> tags;
		for (int i = 1; i <= n; i++) {
			tags.push_back(lua_tostring(L, i));
		}
		XTrace::StartTrace(tags);
	}

	return 0;
}

int IsTracing(lua_State* L) {
	bool isTracing = XTrace::IsTracing();
	lua_pushboolean(L, isTracing);
	return 1;
}

int LogLocal(lua_State* L) {
	int n = lua_gettop(L);

	if (n == 0) {
		std::cout << "Invalid use of LogLocal\n";
	} else {
		std::string msg(lua_tostring(L,1));
		XTRACE(msg);
	}
	return 0;
}

} // end namespace lua_xtracer
