#include <stdint.h>

uint8_t msb(uint8_t x) {
  return 0 - (x >> (8 * sizeof(x) - 1));
}

uint8_t lt(uint8_t a, uint8_t b) {
  return msb(a ^ ((a ^ b) | ((a - b) ^ b)));
}

uint8_t add(uint8_t a, uint8_t b) {
  return (a + b) | lt(a + b, a);
}
