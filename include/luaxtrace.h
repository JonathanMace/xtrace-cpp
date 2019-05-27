#ifndef _LUAXTRACE_H_
#define _LUAXTRACE_H_

#ifdef __cplusplus
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#endif
namespace lua_xtracer {

	extern int StartLuaTrace(lua_State* L);
	extern int IsTracing(lua_State* L);
	extern void RegisterXtraceLua(lua_State* L);
	extern int LogLocal(lua_State* L);

}
#ifdef __cplusplus
}
#endif

#endif

