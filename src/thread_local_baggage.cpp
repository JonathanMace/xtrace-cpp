#include "baggage.h"
#include "xtrace.h"


thread_local Baggage current_threads_baggage;

Baggage& ThreadLocalBaggage::Get() { 
	return current_threads_baggage;
}

Baggage ThreadLocalBaggage::Take() {
	XTRACE("ThreadLocalBaggage::Take", {{"Operation", "unset"}});
	Baggage b = current_threads_baggage;
	current_threads_baggage = Baggage();
	return b;
}

Baggage ThreadLocalBaggage::Branch() {
	XTRACE("ThreadLocalBaggage::Branch", {{"Operation", "fork"}});
	return current_threads_baggage;
}

void ThreadLocalBaggage::Join(Baggage &otherBaggage) {
	current_threads_baggage = Baggage::merge(current_threads_baggage, otherBaggage);
	XTRACE("ThreadLocalBaggage::Join", {{"Operation", "fork"}});
}

void ThreadLocalBaggage::Set(Baggage new_baggage) {
	if (current_threads_baggage.atoms.size() == 0) {
		current_threads_baggage = new_baggage; 
		XTRACE("ThreadLocalBaggage::Set", {{"Operation", "set"}});
	} else {
		current_threads_baggage = new_baggage; 		
	}
}

Baggage ThreadLocalBaggage::Swap(Baggage other_baggage) {
	XTRACE("ThreadLocalBaggage::Swap", {{"Operation", "unset"}});
	Baggage prev = current_threads_baggage;
	current_threads_baggage = other_baggage;
	XTRACE("ThreadLocalBaggage::Swap", {{"Operation", "set"}});
	return prev;
}

void ThreadLocalBaggage::Delete() {
	XTRACE("ThreadLocalBaggage::Delete", {{"Operation", "unset"}});
	current_threads_baggage = Baggage();	
}

Baggage& ThreadLocalBaggage::Get(const char* file, int line) { 
	return current_threads_baggage;
}

Baggage ThreadLocalBaggage::Take(const char* file, int line) {
	XTRACE2(file, line, "ThreadLocalBaggage::Take", {{"Operation", "unset"}});
	Baggage b = current_threads_baggage;
	current_threads_baggage = Baggage();
	return b;
}

Baggage ThreadLocalBaggage::Branch(const char* file, int line) {
	XTRACE2(file, line, "ThreadLocalBaggage::Branch", {{"Operation", "fork"}});
	return current_threads_baggage;
}

void ThreadLocalBaggage::Join(Baggage &otherBaggage, const char* file, int line) {
	current_threads_baggage = Baggage::merge(current_threads_baggage, otherBaggage);
	XTRACE2(file, line, "ThreadLocalBaggage::Join", {{"Operation", "fork"}});
}

void ThreadLocalBaggage::Set(Baggage new_baggage, const char* file, int line) {
	if (current_threads_baggage.atoms.size() == 0) {
		current_threads_baggage = new_baggage; 
		XTRACE2(file, line, "ThreadLocalBaggage::Set", {{"Operation", "set"}});
	} else {
		current_threads_baggage = new_baggage; 		
	}
}

Baggage ThreadLocalBaggage::Swap(Baggage other_baggage, const char* file, int line) {
	XTRACE2(file, line, "ThreadLocalBaggage::Swap", {{"Operation", "unset"}});
	Baggage prev = current_threads_baggage;
	current_threads_baggage = other_baggage;
	XTRACE2(file, line, "ThreadLocalBaggage::Swap", {{"Operation", "set"}});
	return prev;
}

void ThreadLocalBaggage::Delete(const char* file, int line) {
	XTRACE2(file, line, "ThreadLocalBaggage::Delete", {{"Operation", "unset"}});
	current_threads_baggage = Baggage();	
}
