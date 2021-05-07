#include <iostream>
#include <vector>
#include <sstream>
#include "baggage.h"
#include "pubsub.h"
#include "xtrace.h"
#include "xtrace_baggage.h"
#include <thread>
#include "lexvarint.h"
#include "baggageprotocol.h"
#include <map>

extern "C" {
	#include "hindsight.h"
}

uint64_t nanos() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    uint64_t nanos = t.tv_sec * 1000000000UL + t.tv_nsec;
    return nanos;
}

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

void spamthread() {
	int ntraces = 1000000;
	int logpertrace = 1000;
	uint64_t last_print = nanos();
	uint64_t print_every = 1000000000UL;
	uint64_t count = 0;
	for (int i = 0; i < ntraces; i++) {
		uint64_t now = nanos();
		if ((now - last_print) > print_every) {
			uint64_t tput = (count * 1000000000UL) / (now - last_print);
			std::cout << "Done " << tput << "/s" << std::endl;
			count = 0;
			last_print = now;
		}

		XTrace::StartTrace("main.cc");
		hindsight_begin(XTraceBaggage::GetTaskID());
		for (int j = 0; j < logpertrace; j++) {
			XTrace::log("This is an xtrace logging statement.  Hello world!");
		}
		hindsight_end();
		XTraceBaggage::Clear();
		count += logpertrace;
	}
}

int main(int argc, char *argv[]) {
	hindsight_init("XTraceExample");

	std::cout << XTrace::IsTracing() << std::endl;
	XTRACE("a");

	XTrace::StartTrace("main.cc");
	std::cout << XTrace::IsTracing() << std::endl;

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


	// Use the auto baggage macro
	Baggage branched_baggage2 = BRANCH_CURRENT_BAGGAGE();
	std::thread branched_thread2([&branched_baggage2]() {
		BAGGAGE(branched_baggage2);

		XTRACE("f");
		XTRACE("g");
	});

	Baggage inline_branched_baggage = BRANCH_CURRENT_BAGGAGE();

	{
		BAGGAGE(inline_branched_baggage);

		XTRACE("hello world");
		XTRACE("goodbyte world");
	}


	XTRACE("d");
	XTRACE("e");

	branched_thread.join();
	branched_thread2.join();

	JOIN_CURRENT_BAGGAGE(branched_baggage);
	JOIN_CURRENT_BAGGAGE(branched_baggage2);	
	JOIN_CURRENT_BAGGAGE(inline_branched_baggage);

	XTRACE("h");

	int numthreads = 8;
	std::vector<std::thread> threads;
	for (int i = 0; i < numthreads; i++) {
		threads.push_back(std::thread(&spamthread));
	}

	for (int i = 0; i < numthreads; i++) {
		threads[i].join();
	}

	PubSub::shutdown();
	PubSub::join();
}
