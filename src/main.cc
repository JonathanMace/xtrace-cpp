#include <iostream>
#include <vector>
#include <sstream>
#include "baggage.h"
#include "pubsub.h"
#include "xtrace.h"
#include "xtrace_baggage.h"
#include "lexvarint.h"
#include "baggageprotocol.h"
#include <map>

void test_vector(std::vector<int> testv) {
	testv[0] = 5;
	std::cout << testv[0] << std::endl;
}

void printvector(std::vector<uint8_t> bytes) {
	for (unsigned i = 0; i < bytes.size(); i++) {
		std::cout << int(bytes[i]) << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char *argv[]) {
	XTRACE("a");

	XTrace::StartTrace("main.cc");

	std::map<std::string, std::string> mymap = {{"key1", "value1"}, {"key2", "value2"}};

	XTRACE("a");
	XTRACE("b");
	XTRACE("c", {{"key1", "value1"}, {"key2", "value2"}});

	Baggage branched_baggage = BRANCH_CURRENT_BAGGAGE();

	std::thread branched_thread([&branched_baggage]() {
		SET_CURRENT_BAGGAGE(branched_baggage);	
		XTRACE("f");
		XTRACE("g");
		branched_baggage = TAKE_CURRENT_BAGGAGE();
	});

	XTRACE("d");
	XTRACE("e");

	branched_thread.join();
	JOIN_CURRENT_BAGGAGE(branched_baggage);

	XTRACE("h");

	PubSub::shutdown();
	PubSub::join();
}
