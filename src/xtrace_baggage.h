#ifndef _XTRACE_BAGGAGE_H_
#define _XTRACE_BAGGAGE_H_


#include "baggage.h"

namespace XTraceBaggage {

void Clear();
bool HasTaskID();
uint64_t GetTaskID();
void SetTaskID(uint64_t taskId);

std::vector<uint64_t> GetParentEventIDs();
void SetParentEventID(uint64_t eventId);

}

#endif