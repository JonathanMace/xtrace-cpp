#ifndef _BAGGAGE_PROTOCOL_H_
#define _BAGGAGE_PROTOCOL_H_

#include "baggage.h"
#include "lexvarint.h"

#define ATOM_TYPE_MASK 0x80
#define ATOM_TYPE_HEADER 0x80
#define ATOM_TYPE_DATA 0x00

#define HEADER_TYPE_MASK 0x03
#define HEADER_TYPE_INDEXED 0x00
#define HEADER_TYPE_KEYED 0x02

namespace BaggageProtocol {

inline bool IsHeader(Atom &atom){
	return atom.bytes.size() != 0 && (atom.bytes[0] & ATOM_TYPE_MASK) == ATOM_TYPE_HEADER;
}

inline bool IsData(Atom &atom) {
	return atom.bytes.size() != 0 && (atom.bytes[0] & ATOM_TYPE_MASK) == ATOM_TYPE_DATA;
}

inline bool IsIndexedHeader(Atom &atom) {
	return atom.bytes.size() != 0 && (atom.bytes[0] & HEADER_TYPE_MASK) == HEADER_TYPE_INDEXED;
}

inline bool IsKeyedHeader(Atom &atom) {
	return atom.bytes.size() != 0 && (atom.bytes[0] & HEADER_TYPE_MASK) == HEADER_TYPE_KEYED;
}

inline int HeaderLevel(Atom &atom) {
	if (atom.bytes.size() == 0) {
		return -1;
	} else {
		return 15 - int((atom.bytes[0] & 0x78) >> 3);
	}
}

inline uint64_t HeaderIndex(Atom &atom) {
	if (atom.bytes.size() == 0) {
		return -1;
	}
	int n = 1;
	uint64_t header_index = DecodeUnsignedLexVarint(atom.bytes, n);
	return n == 1 ? -1 : header_index;
}

inline std::vector<uint8_t> Payload(Atom &atom) {
	if (atom.bytes.size() > 0) {
		return std::vector<uint8_t>(atom.bytes.begin()+1, atom.bytes.end());	
	}
}

inline std::vector<uint8_t> HeaderKey(Atom &atom) {
	return Payload(atom);
}

inline Atom MakeIndexedHeader(int level, uint64_t index) {
	uint8_t prefix = 0x80 | ((((uint8_t) (15 - level)) << 3) & 0x78) | 0x00;
	std::vector<uint8_t> payload = EncodeUnsignedLexVarint(index);
	payload.insert(payload.begin(), prefix);
	return Atom(payload);
}

inline Atom MakeKeyedHeader(int level, std::vector<uint8_t> key) {
	uint8_t prefix = 0x80 | ((((uint8_t) (15 - level)) << 3) & 0x78) | 0x04;
	key.insert(key.begin(), prefix);
	return Atom(key);
}

inline Atom MakeDataAtom(std::vector<uint8_t> payload) {
	payload.insert(payload.begin(), 0x00);
	return Atom(payload);
}

inline bool findBag(std::vector<Atom> &atoms, Atom bagKey, int &startIndex, int &endIndex) {
	bool foundStart = false;
	while (startIndex < atoms.size() && startIndex < endIndex) {
		int comparison = atoms[startIndex].compare(bagKey);
		if (comparison == 0) {
			foundStart = true;
			break;
		} else if (comparison > 0) {
			endIndex = startIndex;
			break;
		}
		startIndex++;
	}

	if (!foundStart) {
		return false;
	}

	// Find the end of the bag
	int end = startIndex+1;
	while (end < atoms.size() && end < endIndex) {
		int comparison = atoms[end].compare(bagKey);
		if (comparison > 0) {
			break;
		}
		end++;
	}
	endIndex = end;
	return true;
}

inline std::vector<Atom> getBag(std::vector<Atom> &atoms, Atom key) {
	int startIndex = 0;
	int endIndex = atoms.size();
	findBag(atoms, key, startIndex, endIndex);
	if (endIndex > startIndex) {
		return std::vector<Atom>(atoms.begin()+startIndex+1, atoms.begin()+endIndex);
	} else {
		return std::vector<Atom>();
	}
}

inline std::vector<Atom> removeBag(std::vector<Atom> &atoms, Atom key) {
	int startIndex = 0;
	int endIndex = atoms.size();
	if (findBag(atoms, key, startIndex, endIndex)) {
		std::vector<Atom> bag(atoms.begin()+startIndex+1, atoms.begin()+endIndex);
		atoms.erase(atoms.begin()+startIndex, atoms.begin()+endIndex);
		return bag;
	} else {
		return std::vector<Atom>();
	}
}

}

#endif