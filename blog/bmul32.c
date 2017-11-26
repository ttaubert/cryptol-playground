#include <stdint.h>

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

uint64_t bmul32_ret(uint32_t x, uint32_t y) {
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

    return z0 | z1 | z2 | z3;
}
