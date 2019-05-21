#ifndef _LEXVARINT_H_
#define _LEXVARINT_H_

#include <cstdint>
#include <vector>

int SizeUnsignedLexVarint(uint64_t v);

int SizeSignedLexVarint(int64_t v);

std::vector<uint8_t> EncodeUnsignedLexVarint(uint64_t v);

std::vector<uint8_t> EncodeSignedLexVarint(int64_t v);

uint64_t DecodeUnsignedLexVarint(std::vector<uint8_t> &bytes, int &n);
int64_t DecodeSignedLexVarint(std::vector<uint8_t> &bytes, int &n);

#endif