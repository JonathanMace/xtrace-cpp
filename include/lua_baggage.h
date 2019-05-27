#pragma once

#ifndef _LUA_BAGGAGE_H_
#define _LUA_BAGGAGE_H_

extern "C" {
#include <lauxlib.h>
#include <lua.h>
} //extern "C"

namespace lua_xtracer {

	// All the baggage operations are on the serialized versions of Baggage
	extern int SetBaggage(lua_State* L);
	extern int JoinBaggage(lua_State* L);
	extern int BranchBaggage(lua_State* L);
	extern int DeleteBaggage(lua_State* L);

} //end namespace lua_xtracer

#endif
