#include "baggage.h"


thread_local Baggage current_threads_baggage;

Baggage& ThreadLocalBaggage::Get() { 
	return current_threads_baggage;
}

Baggage ThreadLocalBaggage::Take() {
	Baggage b = current_threads_baggage;
	current_threads_baggage = Baggage();
	return b;
}

Baggage ThreadLocalBaggage::Branch() {
	return current_threads_baggage;
}

void ThreadLocalBaggage::Join(Baggage &otherBaggage) {
	current_threads_baggage = Baggage::merge(current_threads_baggage, otherBaggage);
}

void ThreadLocalBaggage::Set(Baggage new_baggage) { 
	current_threads_baggage = new_baggage; 
}

void ThreadLocalBaggage::Delete() {
	current_threads_baggage = Baggage();	
}
