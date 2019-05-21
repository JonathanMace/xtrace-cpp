#include "baggage.h"

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

std::vector<uint8_t> encodeVarint(uint64_t x) {
	std::vector<uint8_t> s;
	while (x >= 128) {
		uint8_t c = x | 0x80;
		s.push_back(c);
		x >>= 7;
	}
	s.push_back(x);
	return s;
}

uint64_t decodeVarint(std::vector<uint8_t> s, unsigned &n) {
	uint64_t x = 0;
	for (unsigned shift = 0; shift < 64; shift += 7) {
		if (n > s.size()) {
			return 0;
		}
		uint8_t b = s[n];
		n++;
		x |= ((uint64_t) (b & 0x7F)) << shift;
		if ((b & 0x80) == 0) {
			return x;
		}
	}
}


const int Atom::compare(const Atom& other) {
	for (unsigned i = 0; i < bytes.size() && i < other.bytes.size(); i++) {
		if (bytes[i] < other.bytes[i]) {
			return -1;
		} else if (bytes[i] > other.bytes[i]) {
			return 1;
		}
	}
	if (bytes.size() < other.bytes.size()) {
		return -1;
	} else if (bytes.size() > other.bytes.size()) {
		return 1;
	} else {
		return 0;
	}	
}


std::ostream& operator<< ( std::ostream& os, const Atom& atom ) {
	os << "[";
	bool first = true;
	for (unsigned i = 0; i < atom.bytes.size(); i++) {
		if (!first) os << " ";
		first = false;

		os << unsigned(atom.bytes[i]);
	}
	if (first) os << "-";
	os << "]";
}

Baggage Baggage::branch() {
	Baggage duplicate;
	duplicate.atoms = atoms;
	return duplicate;	
}

Baggage Baggage::merge(Baggage& a, Baggage& b) {
	Baggage merged;
	unsigned i = 0, j = 0;
	while (i < a.atoms.size() && j < b.atoms.size()) {
		switch(a.atoms[i].compare(b.atoms[j])) {
			case -1: merged.atoms.push_back(a.atoms[i]); i++; break;
			case 0: merged.atoms.push_back(a.atoms[i]); i++; j++; break;
			case 1: merged.atoms.push_back(b.atoms[j]); j++; break;
		}
	}
	for (; i < a.atoms.size(); i++) {
		merged.atoms.push_back(a.atoms[i]);
	}
	for (; j < b.atoms.size(); j++) {
		merged.atoms.push_back(b.atoms[j]);
	}
	return merged;
}

std::vector<uint8_t> Baggage::serialize() {
	std::vector<uint8_t> bytes;
	for (unsigned i = 0; i < atoms.size(); i++) {
		std::vector<uint8_t> size_prefix = encodeVarint(atoms[i].bytes.size());
		bytes.insert(bytes.end(), size_prefix.begin(), size_prefix.end());
		bytes.insert(bytes.end(), atoms[i].bytes.begin(), atoms[i].bytes.end());
	}
	return bytes;
}

std::string Baggage::str() {
	std::vector<uint8_t> bytes = this->serialize();
	return std::string(bytes.begin(), bytes.end());
}

Baggage Baggage::deserialize(std::string bytes) {
	std::vector<uint8_t> bytev(bytes.begin(), bytes.end());
	return Baggage::deserialize(bytev);
}

Baggage Baggage::deserialize(std::vector<uint8_t> bytes) {
	Baggage baggage;
	unsigned n = 0;
	while (n < bytes.size()) {
		uint64_t next_atom_size = decodeVarint(bytes, n);
		Atom a(std::vector<uint8_t>(bytes.begin() + n, bytes.begin() + n + next_atom_size));
		baggage.atoms.push_back(a);
		n += next_atom_size;
	}
	return baggage;
}


std::ostream& operator<< ( std::ostream& os, const Baggage& baggage ) {
	os << "[";
	bool first = true;
	for (unsigned i = 0; i < baggage.atoms.size(); i++) {
		if (!first) os << " ";
		first = false;

		os << baggage.atoms[i];
	}
	os << "]";
}