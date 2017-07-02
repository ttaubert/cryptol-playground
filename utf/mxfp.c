#include <stdint.h>

#define mp_err uint64_t
#define MP_OKAY 0
#define MP_BADARG 1

// Properties:
// returns 0xFFFFFFFFFFFFFFFF if x >= (1 << 63)
//         0x0000000000000000 otherwise
uint64_t msb(uint64_t x) {
  return 0 - (x >> (8*sizeof(x) - 1));
}

// Properties:
// returns 0xFFFFFFFFFFFFFFFF if x == 0
//         0x0000000000000000 otherwise
uint64_t eq_z(uint64_t x) {
  return msb(~x & (x-1));
}

// Properties:
// returns 0xFFFFFFFFFFFFFFFF if a < b
//         0x0000000000000000 otherwise
uint64_t lt(uint64_t a, uint64_t b) {
  return msb(a ^ ((a ^ b) | ((a - b) ^ b)));
}

// Trivial extensions
#define NE_Z(x) ~eq_z(x)
#define GE(a, b) ~lt(a, b)

// Properties:
//    (*carryOut << 64) + out == a + b + (carryIn & 1)
//    carryOut == 0 or 1
//    returns MP_BADARG if carryIn != 0 or 1
mp_err u64_add(uint64_t a, uint64_t b, uint64_t *c, uint64_t carryIn, uint64_t *carryOut) {
  mp_err err = GE(carryIn, 2) & MP_BADARG;
  *c = a + b + carryIn;
  *carryOut = msb( GE(a, 0 - b - carryIn) & (NE_Z(b) | NE_Z(carryIn)) ) & 1;
  return err;
}

// Properties:
//    (*carryOut << 64) + a - b - carryIn = out
//    carryOut == 0 or 1
//    returns MP_BADARG if carryIn != 0 or 1
mp_err u64_sub(uint64_t a, uint64_t b, uint64_t *c, uint64_t carryIn, uint64_t *carryOut) {
  mp_err err = GE(carryIn, 2) & MP_BADARG;
  *c = a - b - carryIn;
  *carryOut = (a >= b + carryIn) ? 0 : 1;
  return err;
}

// Property: (hi << 64) + lo = a * b
mp_err u64_mul(uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo) {
  uint64_t a1 = a >> 32, a0 = a & 0xFFFFFFFF;
  uint64_t b1 = b >> 32, b0 = b & 0xFFFFFFFF;
  uint64_t z0 = a0 * b0;
  uint64_t z2 = a1 * b1;

  uint64_t z1, z1carry, carry, trash;
  u64_add(a0 * b1, a1 * b0, &z1, 0, &z1carry);
  u64_add(z1 << 32, z0, lo, 0, &carry);
  u64_add(z2, z1 >> 32, hi, carry, &trash); // XXX trash?
  u64_add(*hi, z1carry << 32, hi, 0, &trash); // XXX trash?*/

  return MP_OKAY;
}

mp_err u64_square(uint64_t x, uint64_t *hi, uint64_t *lo) {
  return u64_mul(x, x, hi, lo);
}
