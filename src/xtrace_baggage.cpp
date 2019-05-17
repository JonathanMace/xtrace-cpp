#include <iostream>
#include "xtrace_baggage.h"
#include "baggageprotocol.h"

Atom XTraceBagHeader = BaggageProtocol::MakeIndexedHeader(0, 5);
Atom XTraceTaskIDHeader = BaggageProtocol::MakeIndexedHeader(1, 0);
Atom XTraceParentIDsHeader = BaggageProtocol::MakeIndexedHeader(1, 1);	

std::vector<uint8_t> intToBytes(uint64_t v) {
	std::vector<uint8_t> bs(8);
	for (unsigned i = 0; i < 8; i++) {
		bs[7-i] = (v >> (i * 8));
	}
	return bs;
}

uint64_t bytesToInt(std::vector<uint8_t> bs) {
	uint64_t v = 0;
	for (unsigned i = 0; i < 8; i++) {
		v = (v << 8) + bs[i];
	}
	return v;
}

void XTraceBaggage::Clear() {
	Baggage b = ThreadLocalBaggage::Get();
	BaggageProtocol::removeBag(b.atoms, XTraceBagHeader);
	ThreadLocalBaggage::Set(b);
}

bool XTraceBaggage::HasTaskID() {
	Baggage b = ThreadLocalBaggage::Get();
	std::vector<Atom> xtraceAtoms = BaggageProtocol::getBag(b.atoms, XTraceBagHeader);
	std::vector<Atom> taskIdAtoms = BaggageProtocol::getBag(xtraceAtoms, XTraceTaskIDHeader);
	return taskIdAtoms.size() != 0;
}

uint64_t XTraceBaggage::GetTaskID() {
	Baggage b = ThreadLocalBaggage::Get();
	std::vector<Atom> xtraceAtoms = BaggageProtocol::getBag(b.atoms, XTraceBagHeader);
	std::vector<Atom> taskIdAtoms = BaggageProtocol::getBag(xtraceAtoms, XTraceTaskIDHeader);
	if (taskIdAtoms.size() > 0) {
		return bytesToInt(BaggageProtocol::Payload(taskIdAtoms[0]));
	} else {
		return 0;
	}
}

void XTraceBaggage::SetTaskID(uint64_t taskId) {
	Baggage b = ThreadLocalBaggage::Get();
	
	Baggage xtraceBaggage;
	xtraceBaggage.atoms = BaggageProtocol::removeBag(b.atoms, XTraceBagHeader);
	std::vector<Atom> taskIdAtoms = BaggageProtocol::removeBag(xtraceBaggage.atoms, XTraceTaskIDHeader);

	Baggage newTaskIdAtoms;
	newTaskIdAtoms.atoms.push_back(XTraceBagHeader);
	newTaskIdAtoms.atoms.push_back(XTraceTaskIDHeader);
	newTaskIdAtoms.atoms.push_back(BaggageProtocol::MakeDataAtom(intToBytes(taskId)));

	xtraceBaggage.atoms.insert(xtraceBaggage.atoms.begin(), XTraceBagHeader);

	xtraceBaggage = Baggage::merge(xtraceBaggage, newTaskIdAtoms);
	b = Baggage::merge(b, xtraceBaggage);
	ThreadLocalBaggage::Set(b);
}

std::vector<uint64_t> XTraceBaggage::GetParentEventIDs() {
	Baggage b = ThreadLocalBaggage::Get();
	std::vector<Atom> xtraceAtoms = BaggageProtocol::getBag(b.atoms, XTraceBagHeader);
	std::vector<Atom> parentEventIdAtoms = BaggageProtocol::getBag(xtraceAtoms, XTraceParentIDsHeader);
	std::vector<uint64_t> parentEventIds(parentEventIdAtoms.size());
	for (int i = 0; i < parentEventIdAtoms.size(); i++) {
		parentEventIds[i] = bytesToInt(BaggageProtocol::Payload(parentEventIdAtoms[i]));
	}
	return parentEventIds;
}

void XTraceBaggage::SetParentEventID(uint64_t parentEventId) {
	Baggage b = ThreadLocalBaggage::Get();

	Baggage xtraceBaggage;
	xtraceBaggage.atoms = BaggageProtocol::removeBag(b.atoms, XTraceBagHeader);
	std::vector<Atom> parentEventIdAtoms = BaggageProtocol::removeBag(xtraceBaggage.atoms, XTraceParentIDsHeader);


	Baggage newParentEventIdAtoms;
	newParentEventIdAtoms.atoms.push_back(XTraceBagHeader);
	newParentEventIdAtoms.atoms.push_back(XTraceParentIDsHeader);
	newParentEventIdAtoms.atoms.push_back(BaggageProtocol::MakeDataAtom(intToBytes(parentEventId)));

	xtraceBaggage.atoms.insert(xtraceBaggage.atoms.begin(), XTraceBagHeader);

	xtraceBaggage = Baggage::merge(xtraceBaggage, newParentEventIdAtoms);
	b = Baggage::merge(b, xtraceBaggage);
	ThreadLocalBaggage::Set(b);
}

