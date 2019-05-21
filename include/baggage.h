#ifndef _BAGGAGE_H_
#define _BAGGAGE_H_

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <vector>


/*
This file defines the main interfaces (Atoms and Baggage) of the Brown Tracing Plane

Most interactions will be with Baggage and ThreadLocalBaggage
*/


// An atom is just an array of bytes
struct Atom {

	std::vector<uint8_t> bytes;

	Atom() : bytes(0) {}
	Atom(int size) : bytes(size) {}
	Atom(std::vector<uint8_t> bytes) : bytes(bytes) {}

	// Lexicographic comparison of two atoms
	const int compare(const Atom& other);

	friend std::ostream& operator<< ( std::ostream& os, const Atom& atom );
};


// Baggage is an array of atoms
struct Baggage {

	std::vector<Atom> atoms;

	Baggage() : atoms(0) {}

	// Duplicates this baggage
	Baggage branch();

	// Lexicographic merge of two baggage instances, as defined in the Brown Tracing Plane
	static Baggage merge(Baggage& a, Baggage& b);

	// Serialize this baggage, as defined in the Brown Tracing Plane (length-prefix each atom in order)
	std::vector<uint8_t> serialize();

	// Deserialize a serialized baggage instance, as defined in the Brown Tracing Plane
	static Baggage deserialize(std::vector<uint8_t> bytes);

	friend std::ostream& operator<< ( std::ostream& os, const Baggage& baggage );

};


// Helper methods for storing a baggage instance in thread-local storage.  Usage is optional, but very useful
namespace ThreadLocalBaggage {

	Baggage& Get();	                     // Get the current thread's baggage
	Baggage Take();                      // Get the current thread's baggage, and clear the thread-local storage
	Baggage Branch();                    // Get a copy of the current thread's baggage

	void Delete();                       // Delete the current thread's baggage
	void Set(Baggage new_baggage);       // Set the current thread's baggage to the provided baggage, destroying the previous baggage
	void Join(Baggage &otherBaggage);    // Merge the current thread's baggage with the provided baggage

}


#endif