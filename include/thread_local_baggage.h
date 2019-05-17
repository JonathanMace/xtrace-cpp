#ifndef _THREAD_LOCAL_BAGGAGE_H_
#define _THREAD_LOCAL_BAGGAGE_H_

#include "atomlayer.h"

namespace ThreadLocalBaggage {

	Baggage& Get();	 // Get the current thread's baggage
	Baggage Take();  // Get the current thread's baggage, and clear the thread-local storage
	Baggage Branch(); // Get a copy of the current thread's baggage

	void Delete();  // Delete the current thread's baggage
	void Set(Baggage new_baggage);
	void Join(Baggage &otherBaggage);

}


#endif