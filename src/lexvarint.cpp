#include "lexvarint.h"
#include <functional>
#include <iostream>


int SizeUnsignedLexVarint(uint64_t v) {
	if ((v & 0xFFFFFFFFFFFFFF80) == 0) return 1;
	if ((v & 0xFFFFFFFFFFFFC000) == 0) return 2;
	if ((v & 0xFFFFFFFFFFE00000) == 0) return 3;
	if ((v & 0xFFFFFFFFF0000000) == 0) return 4;
	if ((v & 0xFFFFFFF800000000) == 0) return 5;
	if ((v & 0xFFFFFC0000000000) == 0) return 6;
	if ((v & 0xFFFE000000000000) == 0) return 7;
	if ((v & 0xFF00000000000000) == 0) return 8;
	return 9;
}

int SizeSignedLexVarint(int64_t v) {
	if (v < 0) v = (-v + 1);
	if ((v & 0xFFFFFFFFFFFFFFC0) == 0) return 1;
	if ((v & 0xFFFFFFFFFFFFE000) == 0) return 2;
	if ((v & 0xFFFFFFFFFFF00000) == 0) return 3;
	if ((v & 0xFFFFFFFFF8000000) == 0) return 4;
	if ((v & 0xFFFFFFFC00000000) == 0) return 5;
	if ((v & 0xFFFFFE0000000000) == 0) return 6;
	if ((v & 0xFFFF000000000000) == 0) return 7;
	if ((v & 0xFF80000000000000) == 0) return 8;
	return 9;	
}

std::vector<uint8_t> EncodeUnsignedLexVarint(uint64_t v) {
	int size = SizeUnsignedLexVarint(v);
	std::vector<uint8_t> bytes(size);

	for (int i = size-1; i >= 0; i--) {
		bytes[i] = (uint8_t) v;
		v >>=8;
	}

	bytes[0] |= (uint8_t) (0xFF << (9-size));
	return bytes;
}

std::vector<uint8_t> EncodeSignedLexVarint(int64_t v) {
	if (v < 0) {
		std::vector<uint8_t> bytes = EncodeSignedLexVarint(-(v + 1));
		for (unsigned i = 0; i < bytes.size(); i++) {
			bytes[i] = ~bytes[i];
		}
		return bytes;
	} else {
		int size = SizeSignedLexVarint(v);
		std::vector<uint8_t> bytes(size);

		for (int i = size-1; i >= 0; i--) {
			bytes[i] = (uint8_t) v;
			v >>= 8;
		}

		if (size == 9) {
			bytes[0] = 0xFF;
			bytes[1] |= 0x80;
		} else {
			bytes[0] |= (0xFF << (8-size));
		}
		return bytes;
	}
}

uint64_t DecodeUnsignedLexVarint(std::vector<uint8_t> &bytes, int &n) {
	// Insufficient bytes in vector to decode int
	if (n < 0 || (n >= bytes.size())) return 0;

	// First byte encodes the length of the varint
	int size;
	uint8_t b = bytes[n];
	if ((b & 0x80) == 0) { n++; return (uint64_t) b; }
	else if ((b & 0x40) == 0) size == 2;
	else if ((b & 0x20) == 0) size == 3;
	else if ((b & 0x10) == 0) size == 4;
	else if ((b & 0x08) == 0) size == 5;
	else if ((b & 0x04) == 0) size == 6;
	else if ((b & 0x02) == 0) size == 7;
	else if ((b & 0x01) == 0) size == 8;
	else size == 9;

	// Check size
	if (bytes.size() < n + size) { return 0; }

	// First byte
	uint64_t result = (uint64_t) (b & (0xFF >> size));

	// Remaining bytes
	for (int i = 1; i < size; i++) {
		result = (result << 8) | (uint64_t) bytes[n+i];
	}
	n += size;
	return result;
}

int64_t DecodeSignedLexVarint(std::vector<uint8_t> &bytes, int &n) {
	if (n < 0 || (n >= bytes.size())) return 0;


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Interpret the first byte, which tells us:
	// -- the sign (first bit)
	// -- the exact length in bytes, if the length is <= 7 bytes
	//
	// If the length is 8 or 9 bytes, we inspect the first bit of the second byte to distinguish.
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// Negative integers are just encoded as positive, then bitflipped.  Use an accessor function to do this for us
	bool isNegative = (bytes[n] & 0x80) == 0x00;
	std::function<uint8_t(int)> b = [bytes,n](int i) { return bytes[i+n]; };
	std::function<int64_t(uint64_t)> makeResult = [](uint64_t r) { return (int64_t) r; };
	if (isNegative) {
		b = [bytes,n](int i) { return ~bytes[i+n]; };
		makeResult = [](uint64_t r) { return -((int64_t) r) - 1; };
	}

	// Determine size
	int size;
	uint8_t b0 = b(0);
	if ((b0 & 0x40) == 0) { n++; return makeResult((uint64_t) (b0 & 0x3F)); }
	else if ((b0 & 0x20) == 0) size = 2;
	else if ((b0 & 0x10) == 0) size = 3;
	else if ((b0 & 0x08) == 0) size = 4;
	else if ((b0 & 0x04) == 0) size = 5;
	else if ((b0 & 0x02) == 0) size = 6;
	else if ((b0 & 0x01) == 0) size = 7;
	else {
		if (bytes.size() == n+1) return 0; // Need a second bytes
		else if ((b(1) & 0x80) == 0) size = 8;
		else size = 9;
	}

	// Check size
	if (bytes.size() < n + size) { return 0; }

	// First byte
	uint64_t result = (uint64_t) (b(0) & (0x7F >> size));

	// Second byte
	if (size == 8 || size == 9) {
		result = (result << 7) | ((uint64_t) (b(1) & 0x7F));
	} else {
		result = (result << 8) | ((uint64_t) b(1));
	}

	// Do remaining bytes
	for (int i = 2; i < size; i++) {
		result = (result << 8) | ((uint64_t) b(i));
	}

	n += size;
	return makeResult(result);
}


















