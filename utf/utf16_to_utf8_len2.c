#include <stdint.h>

uint32_t utf16_to_utf8_len(uint8_t *buf, uint32_t len)
{
  uint32_t i, out = 0;

  for (i = 0; i < len; i += 2) {
    if (buf[i] == 0x00 && (buf[i] & 0x80) == 0x00) {
      out += 1; // One-byte code point.
    } else if (buf[i] < 0x08) {
      out += 2; // Two-byte code point.
    } else if ((buf[i] & 0xFC) == 0xD8) {
      if ((buf[i+2] & 0xFC) == 0xDC && (len - i) > 2) {
        i += 2;
        out += 4; // Surrogate.
      } else {
        return 0; // Invalid encoding.
      }
    } else if ((buf[i] & 0xFC) == 0xDC) {
      return 0; // Invalid encoding.
    } else {
      out += 3; // Three-byte code point.
    }
  }

  return out;
}
