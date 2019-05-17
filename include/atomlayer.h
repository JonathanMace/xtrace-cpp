#ifndef _ATOM_LAYER_H_
#define _ATOM_LAYER_H_

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <vector>


struct Atom {

	std::vector<uint8_t> bytes;

	Atom() : bytes(0) {}

	Atom(int x) : bytes(4) {
		for (unsigned i = 0; i < 4; i++) {
			bytes[3-i] = (x >> (i * 8));
		}
	}

	Atom(std::vector<uint8_t> bytes) : bytes(bytes) {}

	const int compare(const Atom& other);

	friend std::ostream& operator<< ( std::ostream& os, const Atom& atom );

};


struct Baggage {

	std::vector<Atom> atoms;

	Baggage() : atoms(0) {}

	Baggage branch();

	static Baggage merge(Baggage& a, Baggage& b);

	std::vector<uint8_t> serialize();

	static Baggage deserialize(std::vector<uint8_t> bytes);

	friend std::ostream& operator<< ( std::ostream& os, const Baggage& baggage );

};


#endif