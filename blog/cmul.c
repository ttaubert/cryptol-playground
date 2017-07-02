#include <stdint.h>

uint8_t msb(uint8_t x) {
  return 0 - (x >> (8 * sizeof(x) - 1));
}

uint8_t nz(uint8_t x) {
  return ~msb(~x & (x - 1));
}

uint8_t ge(uint8_t a, uint8_t b) {
  return ~msb(a ^ ((a ^ b) | ((a - b) ^ b)));
}

uint8_t add(uint8_t a, uint8_t b, uint8_t *carry) {
  *carry = msb(ge(a, 0 - b) & nz(b)) & 1;
  return a + b;
}

void mul(uint8_t a, uint8_t b, uint8_t *hi, uint8_t *lo) {
  uint8_t a1 = a >> 4, a0 = a & 0xf;
  uint8_t b1 = b >> 4, b0 = b & 0xf;
  uint8_t z0 = a0 * b0;
  uint8_t z2 = a1 * b1;

  uint8_t z1, z1carry, carry, trash;
  z1 = add(a0 * b1, a1 * b0, &z1carry);
  *lo = add(z1 << 4, z0, &carry);
  *hi = add(z2, (z1 >> 4) + carry, &trash);
  *hi = add(*hi, z1carry << 4, &trash);
}
