#ifndef MULS_H
#define MULS_H
#include <cstdint>
#include <stdlib.h>
using std::uint16_t;
using std::uint64_t;
uint16_t mul8u_1JFF(uint8_t a, uint8_t b);
uint64_t mul8u_EXZ(const uint64_t B,const uint64_t A);
uint64_t mul8u_2AC(const uint64_t B,const uint64_t A);
uint64_t mul8u_185Q(const uint64_t B,const uint64_t A);
uint64_t mul8u_FTA(const uint64_t B,const uint64_t A);

#endif
