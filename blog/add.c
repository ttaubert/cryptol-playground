#include <stdint.h>

uint8_t add(uint8_t a, uint8_t b) {
  uint8_t sum = a + b;
  return sum < a ? UINT8_MAX : sum;
}
