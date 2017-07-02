#include <stdint.h>

int32_t asn1_int(uint8_t *buf, uint32_t len)
{
  int32_t ival = 0;
  unsigned long overflow = ((unsigned long)0xffL) << ((sizeof(ival) - 1)*8);

  while (len) {
    if (ival & overflow) {
      if (ival < 0) {
        return INT32_MIN;
      }
      return INT32_MAX;
    }
    ival = ival << 8;
    ival |= *buf++;
    --len;
  }
  return ival;

  /*int32_t ival = 0;
  unsigned long overflow = 0x1ffUL << (((sizeof(ival) - 1) * 8) - 1);
  unsigned long ofloinit;

  if (*buf & 0x80) {
    ival = -1L;
  }
  ofloinit = ival & overflow;

  while (len) {
    if ((ival & overflow) != ofloinit) {
      if (ival < 0) {
        return INT32_MIN;
      }
      return INT32_MAX;
    }
    ival = ival << 8;
    ival |= *buf++;
    --len;
  }

  return ival;*/

  /*int32_t ival = 0;
  uint8_t negative = 0;
  unsigned int originalLength = len;
  unsigned long overflow = 0x1ffUL << (((sizeof(ival) - 1) * 8) - 1);
  unsigned long mask = 1;

  if (*buf & 0x80) {
    negative = 1;
    overflow <<= 1;
  }

  while (len) {
    if ((ival & overflow) != 0) {
      if (negative) {
        return INT32_MIN;
      }
      return INT32_MAX;
    }
    ival = ival << 8;
    ival |= *buf++;
    --len;
  }
  if (negative && ival && (overflow & ival) == 0) {
    mask <<=  ((originalLength  * 8) - 1);
    ival &= ~mask;
    ival -= mask;
  }
  return ival;*/
}
