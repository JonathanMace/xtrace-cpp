#include <iostream>
#include <vector>
#include "atomlayer.h"
#include "pubsub.h"
#include <sstream>
#include "xtrace_baggage.h"
#include "lexvarint.h"
#include "baggageprotocol.h"
#include "thread_local_baggage.h"
#include "xtrace.h"

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

	XTrace::StartTrace("main.cc");

	XTRACE("a");
	XTRACE("b");
	XTRACE("c");

	Baggage branched_baggage = ThreadLocalBaggage::Branch();

	std::thread branched_thread([&branched_baggage]() {
		ThreadLocalBaggage::Set(branched_baggage);	
		XTRACE("f");
		XTRACE("g");
		branched_baggage = ThreadLocalBaggage::Take();
	});

	XTRACE("d");
	XTRACE("e");

	branched_thread.join();
	ThreadLocalBaggage::Join(branched_baggage);

	XTRACE("h");

	PubSub::shutdown();
	PubSub::join();
}
