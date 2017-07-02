#include <stdint.h>
#include <stddef.h>
//#include <stdio.h>

typedef unsigned __int128 uint128_t;

#define MUL(x, y)   ((uint128_t)(x) * (uint128_t)(y))

void mul(uint64_t x, uint64_t y, uint64_t *r_high, uint64_t *r_low) {
    uint64_t x0, x1, x2, x3;
    uint64_t y0, y1, y2, y3;
    uint64_t m1 = (uint64_t)0x1111111111111111;
    uint64_t m2 = (uint64_t)0x2222222222222222;
    uint64_t m4 = (uint64_t)0x4444444444444444;
    uint64_t m8 = (uint64_t)0x8888888888888888;
    uint128_t z0, z1, z2, z3;
    uint128_t z;

    x0 = x & m1;
    x1 = x & m2;
    x2 = x & m4;
    x3 = x & m8;
    y0 = y & m1;
    y1 = y & m2;
    y2 = y & m4;
    y3 = y & m8;
    z0 = MUL(x0, y0) ^ MUL(x1, y3) ^ MUL(x2, y2) ^ MUL(x3, y1);
    z1 = MUL(x0, y1) ^ MUL(x1, y0) ^ MUL(x2, y3) ^ MUL(x3, y2);
    z2 = MUL(x0, y2) ^ MUL(x1, y1) ^ MUL(x2, y0) ^ MUL(x3, y3);
    z3 = MUL(x0, y3) ^ MUL(x1, y2) ^ MUL(x2, y1) ^ MUL(x3, y0);
    z0 &= ((uint128_t)m1 << 64) | m1;
    z1 &= ((uint128_t)m2 << 64) | m2;
    z2 &= ((uint128_t)m4 << 64) | m4;
    z3 &= ((uint128_t)m8 << 64) | m8;
    z = z0 | z1 | z2 | z3;
    *r_high = (uint64_t)(z >> 64);
    *r_low = (uint64_t)z;
}

void mul_new(uint64_t x, uint64_t y, uint64_t *r_high, uint64_t *r_low) {
    uint128_t x1, x2, x3, x4, x5;
    uint128_t y1, y2, y3, y4, y5;
    uint128_t r, z;

    uint128_t m1 = (uint128_t)0x2108421084210842 << 64 | 0x1084210842108421;
    uint128_t m2 = (uint128_t)0x4210842108421084 << 64 | 0x2108421084210842;
    uint128_t m3 = (uint128_t)0x8421084210842108 << 64 | 0x4210842108421084;
    uint128_t m4 = (uint128_t)0x0842108421084210 << 64 | 0x8421084210842108;
    uint128_t m5 = (uint128_t)0x1084210842108421 << 64 | 0x0842108421084210;

    x1 = x & m1;
    y1 = y & m1;
    x2 = x & m2;
    y2 = y & m2;
    x3 = x & m3;
    y3 = y & m3;
    x4 = x & m4;
    y4 = y & m4;
    x5 = x & m5;
    y5 = y & m5;

    z = (x1 * y1) ^ (x2 * y5) ^ (x3 * y4) ^ (x4 * y3) ^ (x5 * y2);
    r = z & m1;
    z = (x1 * y2) ^ (x2 * y1) ^ (x3 * y5) ^ (x4 * y4) ^ (x5 * y3);
    r |= z & m2;
    z = (x1 * y3) ^ (x2 * y2) ^ (x3 * y1) ^ (x4 * y5) ^ (x5 * y4);
    r |= z & m3;
    z = (x1 * y4) ^ (x2 * y3) ^ (x3 * y2) ^ (x4 * y1) ^ (x5 * y5);
    r |= z & m4;
    z = (x1 * y5) ^ (x2 * y4) ^ (x3 * y3) ^ (x4 * y2) ^ (x5 * y1);
    r |= z & m5;

    *r_high = (uint64_t)(r >> 64);
    *r_low = (uint64_t)r;
}

void bmul32(uint32_t x, uint32_t y, uint32_t *r_high, uint32_t *r_low) {
    uint32_t x0, x1, x2, x3;
    uint32_t y0, y1, y2, y3;
    uint32_t m1 = (uint32_t)0x11111111;
    uint32_t m2 = (uint32_t)0x22222222;
    uint32_t m4 = (uint32_t)0x44444444;
    uint32_t m8 = (uint32_t)0x88888888;
    uint64_t z0, z1, z2, z3;
    uint64_t z;

    x0 = x & m1;
    x1 = x & m2;
    x2 = x & m4;
    x3 = x & m8;
    y0 = y & m1;
    y1 = y & m2;
    y2 = y & m4;
    y3 = y & m8;
    z0 = ((uint64_t)x0 * y0) ^ ((uint64_t)x1 * y3) ^
         ((uint64_t)x2 * y2) ^ ((uint64_t)x3 * y1);
    z1 = ((uint64_t)x0 * y1) ^ ((uint64_t)x1 * y0) ^
         ((uint64_t)x2 * y3) ^ ((uint64_t)x3 * y2);
    z2 = ((uint64_t)x0 * y2) ^ ((uint64_t)x1 * y1) ^
         ((uint64_t)x2 * y0) ^ ((uint64_t)x3 * y3);
    z3 = ((uint64_t)x0 * y3) ^ ((uint64_t)x1 * y2) ^
         ((uint64_t)x2 * y1) ^ ((uint64_t)x3 * y0);
    z0 &= ((uint64_t)m1 << 32) | m1;
    z1 &= ((uint64_t)m2 << 32) | m2;
    z2 &= ((uint64_t)m4 << 32) | m4;
    z3 &= ((uint64_t)m8 << 32) | m8;
    z = z0 | z1 | z2 | z3;
    *r_high = (uint32_t)(z >> 32);
    *r_low = (uint32_t)z;
}

void mul2(uint64_t x, uint64_t y, uint64_t *r_high, uint64_t *r_low) {
  uint128_t z = 0;
  uint128_t a = x;
  uint128_t b = y;

  while (b) {
    if (b & 1) {
      z ^= a;
    }

    a <<= 1;
    b >>= 1;
  }

  *r_high = (uint64_t)(z >> 64);
  *r_low = (uint64_t)z;
}

uint64_t
get64(const unsigned char *bytes)
{
    return ((uint64_t)bytes[0]) << 56 |
           ((uint64_t)bytes[1]) << 48 |
           ((uint64_t)bytes[2]) << 40 |
           ((uint64_t)bytes[3]) << 32 |
           ((uint64_t)bytes[4]) << 24 |
           ((uint64_t)bytes[5]) << 16 |
           ((uint64_t)bytes[6]) << 8 |
           ((uint64_t)bytes[7]);
}

void gcm_HashMult_sftw32(uint64_t h_high, uint64_t h_low,
                         uint64_t x_high, uint64_t x_low,
                         const unsigned char *buf,
                         uint64_t *r_high, uint64_t *r_low)
{
    uint64_t ci_low, ci_high;
    uint64_t z_high_h, z_high_l, z_low_h, z_low_l;
    uint32_t ci_high_h, ci_high_l, ci_low_h, ci_low_l;
    uint32_t b_a_h, b_a_l, a_a_h, a_a_l, b_b_h, b_b_l;
    uint32_t a_b_h, a_b_l, b_c_h, b_c_l, a_c_h, a_c_l, c_c_h, c_c_l;
    uint32_t ci_highXlow_h, ci_highXlow_l, c_a_h, c_a_l, c_b_h, c_b_l;

    uint32_t h_high_h = (uint32_t)(h_high >> 32);
    uint32_t h_high_l = (uint32_t)h_high;
    uint32_t h_low_h = (uint32_t)(h_low >> 32);
    uint32_t h_low_l = (uint32_t)h_low;
    uint32_t h_highXlow_h = h_high_h ^ h_low_h;
    uint32_t h_highXlow_l = h_high_l ^ h_low_l;

    ci_low = x_low ^ get64(buf + 8);
    ci_high = x_high ^ get64(buf);
    ci_low_h = (uint32_t)(ci_low >> 32);
    ci_low_l = (uint32_t)ci_low;
    ci_high_h = (uint32_t)(ci_high >> 32);
    ci_high_l = (uint32_t)ci_high;
    ci_highXlow_h = ci_high_h ^ ci_low_h;
    ci_highXlow_l = ci_high_l ^ ci_low_l;

    /* Do binary mult ghash->X = C * ghash->H (double Karatsuba). */
    bmul32(ci_high_h, h_high_h, &a_a_h, &a_a_l);
    bmul32(ci_high_l, h_high_l, &a_b_h, &a_b_l);
    bmul32(ci_high_h ^ ci_high_l, h_high_h ^ h_high_l, &a_c_h, &a_c_l);
    a_c_h ^= a_a_h ^ a_b_h;
    a_c_l ^= a_a_l ^ a_b_l;
    a_a_l ^= a_c_h;
    a_b_h ^= a_c_l;
    /* ci_high * h_high = a_a_h:a_a_l:a_b_h:a_b_l */

    bmul32(ci_low_h, h_low_h, &b_a_h, &b_a_l);
    bmul32(ci_low_l, h_low_l, &b_b_h, &b_b_l);
    bmul32(ci_low_h ^ ci_low_l, h_low_h ^ h_low_l, &b_c_h, &b_c_l);
    b_c_h ^= b_a_h ^ b_b_h;
    b_c_l ^= b_a_l ^ b_b_l;
    b_a_l ^= b_c_h;
    b_b_h ^= b_c_l;
    /* ci_low * h_low = b_a_h:b_a_l:b_b_h:b_b_l */

    bmul32(ci_highXlow_h, h_highXlow_h, &c_a_h, &c_a_l);
    bmul32(ci_highXlow_l, h_highXlow_l, &c_b_h, &c_b_l);
    bmul32(ci_highXlow_h ^ ci_highXlow_l, h_highXlow_h ^ h_highXlow_l, &c_c_h, &c_c_l);
    c_c_h ^= c_a_h ^ c_b_h;
    c_c_l ^= c_a_l ^ c_b_l;
    c_a_l ^= c_c_h;
    c_b_h ^= c_c_l;
    /* (ci_high ^ ci_low) * (h_high ^ h_low) = c_a_h:c_a_l:c_b_h:c_b_l */

    c_a_h ^= b_a_h ^ a_a_h;
    c_a_l ^= b_a_l ^ a_a_l;
    c_b_h ^= b_b_h ^ a_b_h;
    c_b_l ^= b_b_l ^ a_b_l;
    z_high_h = ((uint64_t)a_a_h << 32) | a_a_l;
    z_high_l = (((uint64_t)a_b_h << 32) | a_b_l) ^
               (((uint64_t)c_a_h << 32) | c_a_l);
    z_low_h = (((uint64_t)b_a_h << 32) | b_a_l) ^
              (((uint64_t)c_b_h << 32) | c_b_l);
    z_low_l = ((uint64_t)b_b_h << 32) | b_b_l;

    /* Shift one (multiply by x) as gcm spec is stupid. */
    z_high_h = z_high_h << 1 | z_high_l >> 63;
    z_high_l = z_high_l << 1 | z_low_h >> 63;
    z_low_h = z_low_h << 1 | z_low_l >> 63;
    z_low_l <<= 1;

    /* Reduce */
    z_low_h ^= (z_low_l << 63) ^ (z_low_l << 62) ^ (z_low_l << 57);
    z_high_h ^= z_low_h ^ (z_low_h >> 1) ^ (z_low_h >> 2) ^ (z_low_h >> 7);
    z_high_l ^= z_low_l ^ (z_low_l >> 1) ^ (z_low_l >> 2) ^ (z_low_l >> 7) ^
                (z_low_h << 63) ^ (z_low_h << 62) ^ (z_low_h << 57);

    *r_high = z_high_h;
    *r_low = z_high_l;
}

void gcm_HashMult_sftw(uint64_t h_high, uint64_t h_low,
                       uint64_t x_high, uint64_t x_low,
                       const unsigned char *buf, size_t count,
                       uint64_t *r_high, uint64_t *r_low)
{
    uint64_t ci_low, ci_high;
    size_t i;
    uint64_t z2_low, z2_high, z0_low, z0_high, z1a_low, z1a_high;
    uint128_t z_high = 0, z_low = 0;

    ci_low = x_low;
    ci_high = x_high;
    for (i = 0; i < count; i++, buf += 16) {
        ci_low ^= get64(buf + 8);
        ci_high ^= get64(buf);

        /* Do binary mult ghash->X = C * ghash->H (Karatsuba). */
        mul_new(ci_high, h_high, &z2_high, &z2_low);
        mul_new(ci_low, h_low, &z0_high, &z0_low);
        mul_new(ci_high ^ ci_low, h_high ^ h_low, &z1a_high, &z1a_low);
        z1a_high ^= z2_high ^ z0_high;
        z1a_low ^= z2_low ^ z0_low;
        z_high = ((uint128_t)z2_high << 64) | (z2_low ^ z1a_high);
        z_low = (((uint128_t)z0_high << 64) | z0_low) ^ (((uint128_t)z1a_low) << 64);

        /* Shift one (multiply by x) as gcm spec is stupid. */
        z_high = (z_high << 1) | (z_low >> 127);
        z_low <<= 1;

        /* Reduce */
        z_low ^= (z_low << 127) ^ (z_low << 126) ^ (z_low << 121);
        z_high ^= z_low ^ (z_low >> 1) ^ (z_low >> 2) ^ (z_low >> 7);
        ci_low = (uint64_t)z_high;
        ci_high = (uint64_t)(z_high >> 64);
    }

    *r_high = ci_high;
    *r_low = ci_low;
}

/*int main() {
  uint64_t a = 18432945115807726987U, b = 18226049434058520172U, hi, lo;
  mul(a, b, &hi, &lo);
  printf("mul(a, b) = hi=%llu lo=%llu\n", hi, lo);
  mul2(a, b, &hi, &lo);
  printf("mul2(a, b) = hi=%llu lo=%llu\n", hi, lo);
}*/
