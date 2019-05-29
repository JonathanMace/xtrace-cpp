#include "baggage.h"
#include "lua_baggage.h"
#include <string>
#include <iostream>

namespace lua_xtracer {

Baggage getBaggageFromLua(lua_State* L) {
	std::string baggage_string = "";

	int n = lua_gettop(L);

	if (n == 0) { 
		std::cout << "Baggage not provided as an argument" << std::endl;
	} else {
		size_t len;
		const char* baggage = lua_tolstring(L, 1, &len);
                baggage_string.assign(baggage, baggage + len);
	}

	Baggage b = Baggage::deserialize(baggage_string);
	return b;
}

int SetBaggage(lua_State* L) {
	Baggage b = getBaggageFromLua(L);
	SET_CURRENT_BAGGAGE(b);
	return 0;
}

int JoinBaggage(lua_State* L) {
	Baggage b = getBaggageFromLua(L);
	JOIN_CURRENT_BAGGAGE(b);
	return 0;
}

int DeleteBaggage(lua_State* L) {
	DELETE_CURRENT_BAGGAGE();
	return 0;
}

int BranchBaggage(lua_State* L) {
	Baggage b = BRANCH_CURRENT_BAGGAGE();
	std::string serialized_baggage = b.str();
	lua_pushlstring(L, serialized_baggage.c_str(), serialized_baggage.length());
	return 1;
}

} //end namespace lua_xtracer
